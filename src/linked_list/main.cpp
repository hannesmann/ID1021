#include <chrono>
#include <memory>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "stack.h"

#define REPEATS 250

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

/* The standard library is too good... */
inline void naive_memset(char* array, char value, int n) {
	for(int i = 0; i < n; i++) {
		array[i] = value;
	}
}

inline void naive_memcpy(char* dest, const char* src, int n) {
	for(int i = 0; i < n; i++) {
		dest[i] = src[i];
	}
}

void benchmark_append_both(bool fixed_a, bool fixed_b) {
	/* 2^8..2^16 */
	for(int i = 8; i < 17; i++) {
		int n = 1 << i;

		LinkedList<char>* a = new LinkedList<char>('A');
		LinkedList<char>* b = new LinkedList<char>('B');

		LinkedList<char>* tail = a;
		for(int j = 0; j < (fixed_a ? 512 : n); j++) {
			tail = tail->tail = new LinkedList<char>('A');
		}
		tail = b;
		for(int j = 0; j < (fixed_b ? 512 : n); j++) {
			tail = tail->tail = new LinkedList<char>('B');
		}

		LinkedList<char>* b_orig_tail = b->last();

		long tstart = time_ns();
		for(int repeat = 0; repeat < REPEATS; repeat++) {
			/* reset */
			b_orig_tail->tail = nullptr;
			/* append again */
			b->append(a);
		}

		printf("  (linked list) n=%d: %lf ns\n", n, (time_ns() - tstart) / (double)REPEATS);

		LinkedList<char>* last = b;
		while(b) {
			last = b;
			b = b->tail;
			delete last;
		}
	}

	printf("\n");

	for(int i = 8; i < 17; i++) {
		int n = 1 << i;
		int n_a = (fixed_a ? 512 : n);
		int n_b = (fixed_b ? 512 : n);

		char* a = new char[n_a];
		char* b = new char[n_b];

		memset(a, 'A', n_a);
		memset(b, 'B', n_b);

		/* To make the benchmark as fair as possible, the array is deleted AFTER the benchmark (same as with the linked list) */
		char** delete_queue = new char*[REPEATS];

		long tstart = time_ns();
		for(int repeat = 0; repeat < REPEATS; repeat++) {
			char* c = new char[n_a + n_b];
			naive_memcpy(c, b, n_b);
			naive_memcpy(c+n_b, a, n_a);

			delete_queue[repeat] = c;
		}

		printf("  (array) n=%d: %lf ns\n", n, (time_ns() - tstart) / (double)REPEATS);

		for(int j = 0; j < REPEATS; j++) {
			delete delete_queue[j];
		}

		delete[] delete_queue;
		delete[] b;
		delete[] a;
	}
}

void benchmark_allocate_both() {
	for(int i = 8; i < 17; i++) {
		int n = 1 << i;

		long tstart = time_ns();
		LinkedList<char>* list = new LinkedList<char>(0);

		for(int j = 0; j < n; j++) {
			list->append(new LinkedList<char>(0));
		}

		LinkedList<char>* last = list;
		while(list) {
			last = list;
			list = list->tail;
			delete last;
		}

		printf("  (linked list) n=%d: %ld ns\n", n, time_ns() - tstart);
	}

	printf("\n");

	for(int i = 8; i < 17; i++) {
		int n = 1 << i;

		long tstart = time_ns();
		for(int repeat = 0; repeat < REPEATS; repeat++) {
			LinkedList<char>* list = new LinkedList<char>(1);
			LinkedList<char>* tail = list;

			for(int j = 0; j < n; j++) {
				tail = tail->tail = new LinkedList<char>(1);
			}

			LinkedList<char>* last = list;
			while(list) {
				last = list;
				list = list->tail;
				delete last;
			}
		}

		printf("  (linked list smart) n=%d: %lf ns\n", n, (time_ns() - tstart) / (double)REPEATS);
	}

	printf("\n");

	for(int i = 8; i < 17; i++) {
		int n = 1 << i;

		long tstart = time_ns();
		for(int repeat = 0; repeat < REPEATS; repeat++) {
			char* list = new char[n];
			naive_memset(list, 1, n);
			delete[] list;
		}

		printf("  (array) n=%d: %lf ns\n", n, (time_ns() - tstart) / (double)REPEATS);
	}
}

int main(int argc, char** argv) {
	srand(time_ns());

	printf("Appending fixed size list (512) to variable size list (256...65536)...\n");
	benchmark_append_both(true, false);

	printf("\nAppending variable size list (256...65536) to fixed size list (512)...\n");
	benchmark_append_both(false, true);

	printf("\nAllocating arrays and linked lists...\n");
	benchmark_allocate_both();

	printf("\nTesting stack...\n");

	LinkedListStack stack;

	stack.push(5);
	stack.push(10);
	assert(stack.pop() == 10);

	stack.push(100);
	assert(stack.pop() == 100);
	assert(stack.pop() == 5);

	printf("All good!\n");

	return 0;
}

