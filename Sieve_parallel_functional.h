#pragma once
#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <sstream>

#define MAX 50000000
#define MIN 25000000
#define NUM_THREADS 4

class Sieve_parallel_functional
{
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


	int* sieve_init(int min = MIN, int max = MAX) {

		omp_set_num_threads(NUM_THREADS);

		int i, j;

		int max_sqrt = ceil(sqrt(max));

		int* numbers = (int*)malloc((max-min+1) * sizeof(int*));
		int* primaries = traditional_sequential(2, ceil(sqrt(max)));

#pragma omp parallel for
		for (int i = 0; i < max-min+1; i++) {
			numbers[i] = min+i;
		}


#pragma omp parallel sections private(i,j)
		{

#pragma omp section
			{
				int th_nb = omp_get_thread_num();

				for (i = 2; i <= 2; i++) {
					if (primaries[i - 2]) {
						for (j = primaries[i - 2] * primaries[i - 2]; j <= max; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
							if (j >= min) {
								numbers[j - min] = 0;
							}
						}
					}
				}
			}
#pragma omp section
			{

				for (i = 3; i <= 5; i++) {
					if (primaries[i - 2]) {
						for (j = primaries[i - 2] * primaries[i - 2]; j <= max; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
							if (j >= min) {
								numbers[j - min] = 0;
							}
						}
					}
				}
			}
#pragma omp section
			{
				for (i = 7; i <= 13; i++) {
					if (primaries[i - 2]) {
						for (j = primaries[i - 2] * primaries[i - 2]; j <= max; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
							if (j >= min) {
								numbers[j - min] = 0;
							}
						}
					}
				}
			}
#pragma omp section
			{
				for (i = 17; i <= max_sqrt; i++) {
					if (primaries[i - 2]) {
						for (j = primaries[i - 2] * primaries[i - 2]; j <= max; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
							if (j >= min) {
								numbers[j - min] = 0;
							}
						}
					}
				}

			}
		}

		return numbers;
	}

	int main() {



		return 1;
	}


	//fragment tablicy liczb pierwszych + wszystkie wykreœlenia
	int* sieve_parallel_functional(int min = MIN, int max = MAX) {

		omp_set_num_threads(NUM_THREADS);

		int* numbers = (int*)malloc((max - min + 1) * sizeof(int));

		int i, j;
		int max_sqrt = ceil(sqrt(max));

#pragma omp parallel for private(i)
		for (i = 0; i <= max - min; i++)
			numbers[i] = min + i;

		int* primaries = traditional_sequential(2, ceil(sqrt(max)));


#pragma omp parallel for schedule(static) default(none) private(i) shared(j,max,primaries,numbers,max_sqrt,min,std::cout)
		for (i = 2; i <= max_sqrt; i++) {
			if (primaries[i - 2]) {
				for (j = primaries[i - 2] * primaries[i - 2]; j <= max; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
					if (j >= min) {
						numbers[j - min] = 0;
					}
				}
			}
		}
		//}
	//}
		free(primaries);
		return numbers;
	}
};

