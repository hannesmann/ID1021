#include <chrono>
#include <functional>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "search.h"

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

void fill_array_with_sorted_data(int* data, int n) {
	data[0] = rand() % 10 + 1;
	for(int i = 1; i < n; i++) {
		data[i] = data[i - 1] + rand() % 10 + 1;
	}
}

/* Receives a function b = F(key, array, size). */
void benchmark_search(std::function<bool(int, int*, int)> f) {
	/* 2^14..2^23 */
	for(int i = 14; i < 24; i++) {
		int n = 1 << i;

		/* A sorted array is always used, but the algorithm only assumes the array is sorted if assume_sorted==true */
		int* array = new int[n];
		fill_array_with_sorted_data(array, n);

		long tstart = time_ns();
		for(int repeat = 0; repeat < 250; repeat++) {
			/* Value is increased by 1..11 for every n, so select a value that has a reasonable chance to be present in the array */
			int key = rand() % (n * 5);
			f(key, array, n);
		}

		printf("  n=%d: %lf ns\n", n, (time_ns() - tstart) / 250.0);
		delete[] array;
	}
}

/* Receives a function b = F(key, array, size). */
void benchmark_duplicates(std::function<bool(int, int*, int)> f) {
	static int sizes[] = { 100, 500, 1000, 2000, 3000, 4000, 5000 };

	for(int i = 0; i < sizeof(sizes) / sizeof(int); i++) {
		int n = sizes[i];

		int* array1 = new int[n];
		int* array2 = new int[n];

		long tstart = time_ns();
		for(int repeat = 0; repeat < 100; repeat++) {
			fill_array_with_sorted_data(array1, n);
			fill_array_with_sorted_data(array2, n);

			for(int i = 0; i < n; i++) {
				int key = array1[i];
				f(key, array2, n);
			}
		}
		long tdiff = (time_ns() - tstart);

		long tdummy = time_ns();
		for(int repeat = 0; repeat < 100; repeat++) {
			fill_array_with_sorted_data(array1, n);
			fill_array_with_sorted_data(array2, n);
		}
		long tdiffdummy = (time_ns() - tdummy);

		printf("  n=%d: %lf ns\n", n, (tdiff - tdiffdummy) / 100.0);
		delete[] array2;
		delete[] array1;
	}
}

void benchmark_duplicates_smart() {
	static int sizes[] = { 100, 500, 1000, 2000, 3000, 4000, 5000 };

	for(int i = 0; i < sizeof(sizes) / sizeof(int); i++) {
		int n = sizes[i];

		int* array1 = new int[n];
		int* array2 = new int[n];

		long tstart = time_ns();
		for(int repeat = 0; repeat < 300; repeat++) {
			fill_array_with_sorted_data(array1, n);
			fill_array_with_sorted_data(array2, n);

			int i1 = 0;
			int i2 = 0;

			while(i1 < n && i2 < n) {
				if(array1[i1] > array2[i2]) {
					i2++;
				}
				else {
					if(array1[i1] == array2[i2]) {
						/* found duplicate */
					}

					i1++;
				}
			}
		}
		long tdiff = (time_ns() - tstart);

		long tdummy = time_ns();
		for(int repeat = 0; repeat < 300; repeat++) {
			fill_array_with_sorted_data(array1, n);
			fill_array_with_sorted_data(array2, n);
		}
		long tdiffdummy = (time_ns() - tdummy);

		printf("  n=%d: %lf ns\n", n, (tdiff - tdiffdummy) / 300.0);
		delete[] array2;
		delete[] array1;
	}
}

int main(int argc, char** argv) {
	srand(time_ns());

	printf("Testing linear search...\n");
	benchmark_search([] (int key, int* array, int n) { return linear_search(key, array, n, false); });

	printf("\nTesting linear search with sort optimization...\n");
	benchmark_search([] (int key, int* array, int n) { return linear_search(key, array, n, true); });

	printf("\nTesting binary search...\n");
	benchmark_search(&binary_search);

	printf("\nSearching for duplicates with linear search...\n");
	benchmark_duplicates([] (int key, int* array, int n) { return linear_search(key, array, n, false); });

	printf("\nSearching for duplicates with binary search...\n");
	benchmark_duplicates(&binary_search);

	printf("\nSearching for duplicates with 'smart' algorithm...\n");
	benchmark_duplicates_smart();

	return 0;
}