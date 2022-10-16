#include <chrono>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_queue.h"
#include "heap_queue.h"

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

void test_priority_queue(PriorityQueue* queue) {
	printf("  %s... ", queue->describe());

	queue->push(8);
	queue->push(2);
	queue->push(5);

	assert(queue->pop() == 2);
	assert(queue->pop() == 5);

	queue->push(1);
	queue->push(1);

	assert(queue->pop() == 1);
	assert(queue->pop() == 1);
	assert(queue->pop() == 8);
	assert(!queue->pop().has_value());

	delete queue;

	printf("Correct\n");
}

#define REPEATS 60

/* Receives a function q = F(max_elements). */
void benchmark_priority_queue(std::function<PriorityQueue*(int)> f) {
	static int elemcount[] = { 50, 100, 250, 500, 1000, 2500, 5000 };

	for(unsigned long i = 0; i < sizeof(elemcount) / sizeof(int); i++) {
		int n = elemcount[i];
		PriorityQueue* queue = f(n);

		double min = 0;
		double max = 0;

		for(int i = 1; i <= 4; i++) {
			long tstart = time_ns();

			for(int repeat = 0; repeat < REPEATS; repeat++) {
				for(int j = 0; j < n; j++) {
					queue->push(rand() % (10*n));
				}
				for(int j = 0; j < n; j++) {
					queue->pop();
				}
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
		printf("  [%s] n=%d: %.2lf ns (± %.2lf ns)\n", queue->describe(), n, avg / REPEATS, (max - avg) / REPEATS);

		delete queue;
	}

	printf("\n");
}

int main(int argc, char** argv) {
	srand(time_ns());

	printf("Verifying that queues work correctly...\n");
	test_priority_queue(new PriorityListQueue(ListQueue_FastAdd));
	test_priority_queue(new PriorityListQueue(ListQueue_FastRemove));
	test_priority_queue(new HeapQueue());

	printf("Benchmarking queues...\n");
	benchmark_priority_queue([] (int n) { return new PriorityListQueue(ListQueue_FastAdd); });
	benchmark_priority_queue([] (int n) { return new PriorityListQueue(ListQueue_FastRemove); });
	benchmark_priority_queue([] (int n) { return new HeapQueue(); });

	return 0;
}