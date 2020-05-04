#include <stdio.h>
#include <iostream>
#include <omp.h>
#include "Time.h"
#include "Sieve_parallel.h"
#include "Sieve_parallel_functional.h"
#include "Sieve_domain.h"

#define MAX 500000
#define MIN 250000
#define NULL 0 
#define NUM_THREADS 4

int* initialize_array_of_nulls() {
	int* array = (int*)malloc(MAX-MIN);
	for (int i = 0; i < MAX-MIN; i++)
		array[i] = NULL;

	return array;
}

int* initialize_array_from_min_to_max() {
	int* array = (int*)malloc(MAX - MIN);
	for (int i = 0; i < MAX-MIN; i++)
		array[i] = MIN + i;

	return array;
}


int* traditional_parallel() {

	omp_set_num_threads(NUM_THREADS);

	int max = MAX, min = MIN;
	int i, j;
	int max_sqrt = ceil(sqrt(max));

	int* numbers = (int*)malloc((MAX - MIN + 1) * sizeof(int));

	#pragma omp parallel for private(i)
	for (i = 0; i <= MAX - MIN; i++)
		numbers[i] = MIN + i;

	#pragma omp parallel for collapse(2) shared(numbers) private(i,j) 
	for (i = min; i <= max; i++)
	{
		for (j = 2; j <= max_sqrt; j++)
		{
			if (i % j == 0) {
				numbers[i - min] = 0;
			}
		}
	}

	if (min < 3) {
		numbers[0] = 2;
	}

	return numbers;
}

int* traditional_sequential(int min = MIN, int max = MAX) {
	int* numbers = (int*)malloc((max-min+1)*sizeof(int));

	for (int i = 0; i <= max-min; i++)
		numbers[i] = min + i;

	for (int i = min; i <= max; i++) {
		for (int j = 2; j <= ceil(sqrt(i)); j++) {
			if (i % j == 0) {
				numbers[i - min] = 0;
				break;
			}
		}
	}

	if (min < 3) {
		numbers[0] = 2;
	}

	return numbers;
}


int* sieve_sequential(int min = MIN, int max=MAX) {

	int* numbers = (int*)malloc((max-min + 1) * sizeof(int));

	for (int i = 0; i <= max-min; i++)
		numbers[i] = min + i;

	int* primaries = traditional_sequential(2, ceil(sqrt(max)));

	for (int j = 2 * 2; j <= max; j += 2) {
		if (j >= min) {
			numbers[j - min] = 0;
		}
	}

	for (int i = 3; i <= ceil(sqrt(max)); i++) {
		if (primaries[i - 2]) {
			for (int j = primaries[i - 2] * primaries[i - 2]; j <= max; j += 2 * primaries[i - 2]) {
				if (j >= min) {
					numbers[j - min] = 0;
				}
			}
		}
	}

	return numbers;
}

//zwraca tablicê liczb pierwszych, która koñczy siê NULLami
int* first_sequential(int min, int max) {
	int* result = initialize_array_of_nulls();
	
	int count_added = 0;

	if (min == 2) {
		result[count_added++]=2;
	}


	for (int i = min; i <= max; i++) {
		for (int j = 2; j <= ceil(sqrt(i)); j++) {
			if (i % j == 0) {
				break;
			}
			else if (j == ceil(sqrt(i))) {
				result[count_added++]=i;
			}
		}
	}

	return result;
}

//zwraca tablicê liczb pierwszych z wyzerowanymi polami miêdzy nimi
//sito dobre dla w¹skich przedzia³ów które zaczynaj¹ siê od du¿ych liczb
int* second_sequential(int min, int max) {
	//int* result = initialize_array_from_min_to_max(min,max);
	int result[MAX-MIN];
	for (int i = 0; i < MAX - MIN; i++)
		result[i] = NULL;

	int* primaries = first_sequential(2, ceil(sqrt(max)));

	for (int i = 0; primaries[i] != NULL; i++) {
		for (int j = min; result[j-min]!=0 && j <= max; j++) {
			if (result[j-min] % primaries[i] == 0) {
				result[j-min] = 0;
			}
		}
	}

	return result;
}

void print_out_array_with_nulls_at_the_end(int* array) {
	for (int i = 0; i < MAX && array[i] != NULL; i++) {
		std::cout << array[i] << " ";
	}
}

void print_out_array_with_zeros_between(int* array,int max, int min) {
	for (int i = 0; i < max-min; i++) {
		if (array[i]!=0)
			std::cout << array[i] << " ";
	}
}

void print_out_result(int* result) {

	int count_prime_numbers = 0;
	for (int i = 0; i <= MAX - MIN; i++) {
		if (*(result + i)) {
			std::cout << *(result + i) << " ";
			count_prime_numbers++;
			if (count_prime_numbers % 10 == 0)
				std::cout << std::endl;
		}
	}
	std::cout << "Znaleziono liczb pierwszych: " << count_prime_numbers << std::endl;
}


int main()
{
	//traditional_sequential();

	//Sieve* sieve = new Sieve();
	//sieve->sieve_init();

	//Time* time = new Time();
	//traditional_parallel();
	//time->stop();

	//std::cout << time->get() << " \n";

	//Sieve_parallel_functional* sieve_parallel_functional = new Sieve_parallel_functional();
	//sieve_parallel_functional->sieve_parallel_functional();

	Sieve_parallel* sieve_parallel = new Sieve_parallel();
	sieve_parallel->sieve_parallel();
}
