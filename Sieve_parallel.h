#pragma once

#include <stdio.h>
#include <iostream>
#include <omp.h>
#include <mutex>
#include <sstream>

#define MAX 50000000
#define MIN 2
#define NUM_THREADS 2

class Sieve_parallel
{
private:
	static std::mutex mtx_;
public:

	int* traditional_sequential(int min = MIN, int max = MAX) {
		int* numbers = (int*)malloc((max - min + 1) * sizeof(int));

		for (int i = 0; i <= max - min; i++)
			numbers[i] = min + i;

		for (int i = min; i <= max; i++) {
			for (int j = 2; j <= ceil(sqrt(i)); j++) {
				if (i % j == 0) {
					numbers[i - min] = 0;
					break;
				}
				//else if (j = ceil(sqrt(i))) {
				//	std::cout << numbers[i - min] << " ";
				//}
			}
		}

		if (min < 3) {
			numbers[0] = 2;
		}

		return numbers;
	}

	//podej�cie domenowe - procesy otrzymuj� ca�� tablic� liczb pierwszych
//i fragment tablicy wykre�le�
//TODO
	int* sieve_parallel(int min = MIN, int max = MAX) {

		omp_set_num_threads(NUM_THREADS);

		int i, j;

		int max_sqrt = ceil(sqrt(max));

		int* numbers = (int*)malloc((max - min + 1) * sizeof(int));

#pragma omp parallel for private(i)
		for (i = 0; i <= max - min; i++)
			numbers[i] = min + i;

		int* primaries = traditional_sequential(2, ceil(sqrt(max)));

		//procesy otrzymuj� ca�� tablic� liczb pierwszych
		//i dziel� si� dzia�aniami na tablicy wykre�le�
		for (i = 2; i <= max_sqrt; i++) {
			if (primaries[i - 2]) {
#pragma omp parallel for schedule(static) private(j) shared(numbers)
				for (j = i * i; j <= max; j += (i == 2 ? i : 2 * i)) {
					if (j >= min) {
						numbers[j - min] = 0;

					}
				}
			}
		}
		free(primaries);
		return numbers;
	}



	int* sieve(int* primaries, int* numbers, int primaries_length, int numbers_length, int min, int max) {
		for (int i = 0; i < primaries_length; i++) {
			for (int j = primaries[i] * primaries[i]; j <= max; j += 2 * primaries[i]) {
				if (j >= min) {
					numbers[j - min] = 0;
				}
			}
		}
	}

	int main() {

		//std::cout << "Sieve_parallel_domain: " << std::endl;
		int* result = sieve_parallel();

		//for (int i = 0; i < MAX-MIN+1; i++) {
		//	std::cout << result[i] << " ";
		//}

		//eratosthenesOdd(10000, true);

		return 1;
	}
};

