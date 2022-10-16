#include <chrono>
#include <algorithm>
#include <iterator>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sort.h"

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

using namespace std::chrono;
using std::begin, std::end, std::equal;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

/* Make sure our quicksort works */
void sanity_checks() {
	int testA1[] = { 3, 1 };
	int testA2[] = { 3, 1 };

	std::sort(begin(testA1), end(testA1));
	quicksort_array(testA2, 2);

	assert(equal(begin(testA1), end(testA1), begin(testA2)));

	int testA3[] = { 6, 8, 8, 1, 3, 5, 1, 20 };
	int testA4[] = { 6, 8, 8, 1, 3, 5, 1, 20 };

	std::sort(begin(testA3), end(testA3));
	quicksort_array(testA4, 8);

	assert(equal(begin(testA3), end(testA3), begin(testA4)));

	LinkedList<int>* handsorted_linked_list = new LinkedList<int>(1);
	handsorted_linked_list->append_to_end(new LinkedList<int>(3));
	handsorted_linked_list->append_to_end(new LinkedList<int>(3));
	handsorted_linked_list->append_to_end(new LinkedList<int>(100));
	handsorted_linked_list->append_to_end(new LinkedList<int>(204));

	LinkedList<int>* quicksorted_linked_list = new LinkedList<int>(3);
	quicksorted_linked_list->append_to_end(new LinkedList<int>(100));
	quicksorted_linked_list->append_to_end(new LinkedList<int>(1));
	quicksorted_linked_list->append_to_end(new LinkedList<int>(3));
	quicksorted_linked_list->append_to_end(new LinkedList<int>(204));

	quicksorted_linked_list = quicksort_linked_list(quicksorted_linked_list).begin;

	while(handsorted_linked_list || quicksorted_linked_list) {
		assert(handsorted_linked_list->item == quicksorted_linked_list->item);

		handsorted_linked_list = handsorted_linked_list->next;
		quicksorted_linked_list = quicksorted_linked_list->next;
	}
}

void benchmark_quicksort_array(int* random_numbers, int n, int* array, LinkedList<int>**) {
	/* Reset the array */
	for(int i = 0; i < n; i++) {
		array[i] = random_numbers[i];
	}

	quicksort_array(array, n);
}

void benchmark_quicksort_linked_list(int* random_numbers, int n, int*, LinkedList<int>** nodes) {
	/* Reset the linked list - the next pointers will have been scrambled by the previous run but that doesn't matter */
	LinkedList<int>* last = *nodes;
	for(int i = 0; i < n; i++) {
		last->item = random_numbers[i];
		last = last->next;
	}

	*nodes = quicksort_linked_list(*nodes).begin;
}

#define REPEATS 50

/* Receives a function F(random_numbers, n, array_storage, &node_storage) */
void benchmark_quicksort(std::function<void(int*, int n, int*, LinkedList<int>**)> f, bool sorted_data) {
	static int elemcount[] = { 100, 250, 500, 1000, 2500, 5000, 10000 };

	for(unsigned long i = 0; i < sizeof(elemcount) / sizeof(int); i++) {
		int n = elemcount[i];

		int* random_numbers = new int[n];
		for(int j = 0; j < n; j++) {
			random_numbers[j] = rand() % n;
		}

		if(sorted_data) {
			std::sort(random_numbers, random_numbers + n);
		}

		/* Allocate storage for benchmarks to run as quickly as possible */
		int* array = new int[n];

		LinkedList<int>* nodes = new LinkedList<int>(-1);
		LinkedList<int>* last = nodes;
		for(int j = 0; j < n; j++) {
			last = last->next = new LinkedList<int>(-1);
		}

		double min = 0;
		double max = 0;

		for(int i = 1; i <= 4; i++) {
			long tstart = time_ns();

			for(int repeat = 0; repeat < REPEATS; repeat++) {
				f(random_numbers, n, array, &nodes);
			}

			long tdiff = time_ns() - tstart;

			/* First two rounds are warmup */
			if(i >= 2) {
				if(!min || min > tdiff) {
					min = tdiff;
				}
				if(!max || max < tdiff) {
					max = tdiff;
				}
			}
		}

		double avg = (min + max) / 2.0;
		printf("  n=%d: %.2lf ns (± %.2lf ns)\n", n, avg / REPEATS, (max - avg) / REPEATS);

		delete nodes;
		delete[] array;

		delete[] random_numbers;
	}
}

int main(int argc, char** argv) {
	srand(time_ns());

	sanity_checks();

	printf("Benchmarking quicksort with array on random data...\n");
	benchmark_quicksort(&benchmark_quicksort_array, false);
	printf("\nBenchmarking quicksort with linked list on random data...\n");
	benchmark_quicksort(&benchmark_quicksort_linked_list, false);

	printf("\nBenchmarking quicksort with array on sorted data...\n");
	benchmark_quicksort(&benchmark_quicksort_array, true);
	printf("\nBenchmarking quicksort with linked list on sorted data...\n");
	benchmark_quicksort(&benchmark_quicksort_linked_list, true);

	return 0;
}