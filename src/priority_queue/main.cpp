#include <chrono>

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_queue.h"
#include "heap_queue.h"
#include "heap_array_queue.h"

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

#define verify(expr, expected) \
	{  \
		int result = expr;\
		\
		if(result != expected) { \
			printf("Check " #expr " failed: Expected %d, got %d", expected, result); \
			exit(EXIT_FAILURE); \
		} \
	}

void test_priority_queue(PriorityQueue* queue) {
	printf("  %s... ", queue->describe());

	PriorityQueueResult placeholder = { 0, 0 };

	queue->push(8);
	queue->push(2);
	queue->push(5);

	verify(queue->pop().value_or(placeholder).value, 2);
	verify(queue->pop().value_or(placeholder).value, 5);

	queue->push(1);
	queue->push(1);

	verify(queue->pop().value_or(placeholder).value, 1);
	verify(queue->pop().value_or(placeholder).value, 1);
	verify(queue->pop().value_or(placeholder).value, 8);
	verify(!queue->pop().has_value(), true);

	queue->push(4);
	verify(queue->increment(2).value_or(placeholder).value, 4);
	verify(queue->pop().value_or(placeholder).value, 6);
	verify(!queue->pop().has_value(), true);

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

		double sum = 0;
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

			/* First round is warmup */
			if(i > 1) {
				sum += tdiff;

				if(!min || min > tdiff) {
					min = tdiff;
				}
				if(!max || max < tdiff) {
					max = tdiff;
				}
			}
		}

		double avg = sum / 3.0;
		printf("  [%s] n=%d: %.2lf ns (± %.2lf ns)\n", queue->describe(), n, avg / REPEATS, ((max - min) / 2.0) / REPEATS);

		delete queue;
	}

	printf("\n");
}

/* Receives a function q = F(max_elements). */
void benchmark_priority_queue_depth(std::function<PriorityQueue*(int)> f, long seed) {
	static int elemcount[] = { 50, 100, 250, 500, 1000, 2500, 5000 };
	srand(seed);

	for(unsigned long i = 0; i < sizeof(elemcount) / sizeof(int); i++) {
		int n = elemcount[i];

		const char* name;

		int depth_add_sum;
		int min_depth_add;
		int max_depth_add;

		int depth_increment_sum;
		int min_depth_increment;
		int max_depth_increment;

		for(int repeat = 0; repeat < REPEATS; repeat++) {
			PriorityQueue* queue = f(64 + n);

			for(int j = 0; j < 64 + n; j++) {
				queue->push(rand() % 100);
			}

			int depth_add = queue->push(rand() % 100);
			int depth_increment = queue->increment(rand() % 20).value().depth;

			/* On first repeat, init variables */
			if(repeat == 0) {
				name = queue->describe();

				depth_add_sum = min_depth_add = max_depth_add = depth_add;
				depth_increment_sum = min_depth_increment = max_depth_increment = depth_increment;
			}
			else {
				depth_add_sum += depth_add;
				depth_increment_sum += depth_increment;
			}

			min_depth_add = std::min(min_depth_add, depth_add);
			max_depth_add = std::max(max_depth_add, depth_add);

			min_depth_increment = std::min(min_depth_increment, depth_increment);
			max_depth_increment = std::max(max_depth_increment, depth_increment);

			delete queue;
		}

		printf(
			"  [%s] n=%d: push { average=%.2lf,min=%d,max=%d } increment { average=%.2lf,min=%d,max=%d }\n",
			name, n,
			(double)depth_add_sum / REPEATS,
			min_depth_add, max_depth_add,
			(double)depth_increment_sum / REPEATS,
			min_depth_increment,max_depth_increment
		);
	}

	printf("\n");
}

int main(int argc, char** argv) {
	srand(time_ns());

	printf("Verifying that queues work correctly...\n");
	test_priority_queue(new PriorityListQueue(ListQueue_FastAdd));
	test_priority_queue(new PriorityListQueue(ListQueue_FastRemove));
	test_priority_queue(new HeapQueue());
	test_priority_queue(new HeapArrayQueue(0));

	printf("\nBenchmarking queues...\n");
	benchmark_priority_queue([] (int n) { return new PriorityListQueue(ListQueue_FastAdd); });
	benchmark_priority_queue([] (int n) { return new PriorityListQueue(ListQueue_FastRemove); });
	benchmark_priority_queue([] (int n) { return new HeapQueue(); });
	benchmark_priority_queue([] (int n) { return new HeapArrayQueue(n); });

	long seed = time_ns();

	printf("\nBenchmarking queue depth...\n");

	benchmark_priority_queue_depth([] (int n) { return new HeapQueue(); }, seed);
	benchmark_priority_queue_depth([] (int n) { return new HeapArrayQueue(n); }, seed);

	return 0;
}