#include <chrono>
#include <memory>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define REPEATS 200

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

long benchmark_linked_list(LinkedList<char>** elements, int* sequence) {
	LinkedList<char>* first = elements[0];
	long tstart = time_ns();

	for(int repeat = 0; repeat < REPEATS; repeat++) {
		int i = sequence[repeat];

		/* Remove element, if this is the first we move the pointer */
		first = first->remove(elements[i]);

		/* Add back element to front */
		elements[i]->next = first;
		first = elements[i];
	}

	return time_ns() - tstart;
}

long benchmark_doubly_linked_list(DoublyLinkedList<char>** elements, int* sequence) {
	DoublyLinkedList<char>* first = elements[0];
	long tstart = time_ns();

	for(int repeat = 0; repeat < REPEATS; repeat++) {
		int i = sequence[repeat];

		/* Doubly-linked list can handle removing without knowing where the list starts */
		elements[i]->remove_self();

		/* Add back element to front */
		elements[i]->previous = nullptr;
		elements[i]->next = first;

		first->previous = elements[i];
		first = elements[i];
	}

	return time_ns() - tstart;
}

template<typename T>
/* Receives a function t = F(elements, sequence). */
void benchmark_add_remove(std::function<long(T** elements, int* sequence)> f, long seed) {
	static int operations[] = { 5, 50, 100, 1000, 10000, 50000, 100000, 250000 };

	srand(seed);

	for(unsigned long i = 0; i < sizeof(operations) / sizeof(int); i++) {
		int n = operations[i];

		T** elements = new T*[n];
		elements[0] = new T(0);

		for(int j = 1; j < n; j++) {
			elements[j] = new T(j);
			elements[j - 1]->append_to_end(elements[j]);
		}

		/* This will give the same sequence every time as long as the seed remains constant */
		int* sequence = new int[REPEATS];
		for(int j = 0; j < REPEATS; j++) {
			sequence[j] = rand() % n;
		}

		long min = 0;
		long max = 0;

		/* Warmup to get everything in cache */
		for(int i = 0; i < 4; i++) {
			f(elements, sequence);
		}

		for(int i = 0; i < 4; i++) {
			long time = f(elements, sequence);

			if(!min || min > time) {
				min = time;
			}
			if(!max || max < time) {
				max = time;
			}
		}

		double avg = (min + max) / 2.0;
		printf("  n=%d: %lf ns (± %lf ns)\n", n, avg / REPEATS, (max - avg) / REPEATS);

		for(int j = 0; j < n; j++) {
			delete elements[j];
		}

		delete[] elements;
		delete[] sequence;
	}
}

int main(int argc, char** argv) {
	long seed = time_ns();

	printf("Benchmarking remove() with linked list...\n");
	benchmark_add_remove<LinkedList<char>>(&benchmark_linked_list, seed);

	printf("\nBenchmarking remove() with doubly-linked list...\n");
	benchmark_add_remove<DoublyLinkedList<char>>(&benchmark_doubly_linked_list, seed);

	return 0;
}

