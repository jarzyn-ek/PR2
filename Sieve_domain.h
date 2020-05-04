#pragma once
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <sstream>

#define MAX 50000000
#define MIN 2
#define NUM_THREADS 4

class Sieve
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




	int* sieve(int* primaries, int* numbers, int primaries_length, int numbers_length, int min, int max) {
		for (int i = 0; i < primaries_length; i++) {
			for (int j = primaries[i] * primaries[i]; j <= max; j += 2 * primaries[i]) {
				if (j >= min) {
					numbers[j - min] = 0;
				}
			}
		}
	}

	int** sieve_init(int min = MIN, int max = MAX) {

		omp_set_num_threads(NUM_THREADS);
		int in_one_row = ceil((double)(max - min + 1) / (double)NUM_THREADS);

		int i,j;

		int max_sqrt = ceil(sqrt(max));

		int** numbers = (int**)malloc(NUM_THREADS * sizeof(int*));
		int* primaries = traditional_sequential(2, ceil(sqrt(max)));

#pragma omp parallel for
		for (int i = 0; i < NUM_THREADS; i++) {
			numbers[i] = (int*)malloc(in_one_row * sizeof(int));
		}

#pragma omp parallel for
		for (int i = 0; i <= max - min; i++) {
			int row = floor(i / in_one_row);
			int j = i % in_one_row;
			numbers[row][j] = min + i;
			if (i == max - min && j < in_one_row-1) {
				for (j; j < in_one_row; j++) {
					numbers[row][j] = 0;
				}
			}
		}

#pragma omp parallel sections private(i,j)
		{

#pragma omp section
			{
				int th_nb = omp_get_thread_num();
				int maks = numbers[th_nb][in_one_row - 1] == 0 ? max : numbers[th_nb][in_one_row - 1];
				int minm = numbers[th_nb][0];

				for (i = 2; i <= max_sqrt; i++) {
					if (primaries[i - 2]) {
						for (j = primaries[i - 2] * primaries[i - 2]; j <= maks; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
							if (j >= minm) {
								numbers[th_nb][j - minm] = 0;
							}
						}
					}
				}
			}
#pragma omp section
			{
				int th_nb = omp_get_thread_num();
				int maks = numbers[th_nb][in_one_row - 1] == 0 ? max : numbers[th_nb][in_one_row - 1];
				int minm = numbers[th_nb][0];

				for (i = 2; i <= max_sqrt; i++) {
					if (primaries[i - 2]) {
						for (j = primaries[i - 2] * primaries[i - 2]; j <= maks; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
							if (j >= minm) {
								numbers[th_nb][j - minm] = 0;
							}
						}
					}
				}
			}
#pragma omp section
			{
				int th_nb = omp_get_thread_num();
				int maks = numbers[th_nb][in_one_row - 1] == 0 ? max : numbers[th_nb][in_one_row - 1];
				int minm = numbers[th_nb][0];

				for (i = 2; i <= max_sqrt; i++) {
					if (primaries[i - 2]) {
						for (j = primaries[i - 2] * primaries[i - 2]; j <= maks; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
							if (j >= minm) {
								numbers[th_nb][j - minm] = 0;
							}
						}
					}
				}
			}
#pragma omp section
			{
				int th_nb = omp_get_thread_num();
				int maks = numbers[th_nb][in_one_row - 1] == 0 ? max : numbers[th_nb][in_one_row - 1];
				int minm = numbers[th_nb][0];

				for (i = 2; i <= max_sqrt; i++) {
					if (primaries[i - 2]) {
						for (j = primaries[i - 2] * primaries[i - 2]; j <= maks; j += (primaries[i - 2] == 2 ? 2 : 2 * primaries[i - 2])) {
							if (j >= minm) {
								numbers[th_nb][j - minm] = 0;
							}
						}
					}
				}

			}
		}

		return numbers;
	}

};

