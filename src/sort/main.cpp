#include <chrono>

#include <algorithm>
#include <functional>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "algorithms.h"

using namespace std::chrono;

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

void fill_array_with_unsorted_data(int* data, int n) {
	for(int i = 0; i < n; i++) {
		data[i] = rand() % 12;
	}
}

/* Receives a function F(array, size). */
void benchmark_sort(std::function<void(int*, int)> f) {
	/* 2^2..2^12 */
	for(int i = 2; i < 13; i++) {
		int n = 1 << i;

		int* array = new int[n];
		fill_array_with_unsorted_data(array, n);

		long tstart = time_ns();
		for(int repeat = 0; repeat < 200; repeat++) {
			f(array, n);
		}

		printf("  n=2^%d: %lf ns\n", i, (time_ns() - tstart) / 200.0);
		delete[] array;
	}
}

int main(int argc, char** argv) {
	srand(time_ns());

	printf("Testing selection sort...\n");
	benchmark_sort(&selection_sort);

	printf("\nTesting insertion sort...\n");
	benchmark_sort(&insertion_sort);

	printf("\nTesting merge sort...\n");
	benchmark_sort(&merge_sort);

	return 0;
}