#include <chrono>
#include <memory>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "iterator.h"

#include "linked_queue.h"
#include "array_queue.h"
#include "array_queue_virtual.h"

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

/* Test a sequence of pushing and popping with intentional out-of-bounds for the pop() operation */
int test_queue(Queue<int>* queue, int n, long seed) {
	int checksum = 0;
	srand(seed);

	for(int i = 0; i < n / 2; i++) {
		queue->push(rand() % 255);
	}

	for(int i = 0; i < n / 2; i++) {
		if(rand() % 2 == 0) {
			int v = queue->pop().value_or(-1);
			checksum += v;
			printf("%d ", v);
		}
		else {
			queue->push(rand() % 255);
		}
	}

	while(queue->pop().has_value());

	for(int i = 0; i < n; i++) {
		queue->push(rand() % 255);
	}
	for(int i = 0; i < n / 4 + 1; i++) {
		int v = queue->pop().value_or(-1);
		checksum += v;
		printf("%d ", v);
	}

	while(queue->pop().has_value());

	for(int i = 0; i < 4; i++) {
		int v = queue->pop().value_or(-1);
		checksum += v;
		printf("%d ", v);
	}

	printf("\n");

	delete queue;
	return checksum;
}

int main(int argc, char** argv) {
	printf("Testing IntBinaryTreeIterator...\n");
	IntBinaryTree tree;
	tree.add(5, 105);
	tree.add(2, 102);
	tree.add(7, 107);
	tree.add(1, 101);
	tree.add(8, 108);
	tree.add(6, 106);
	tree.add(3, 103);

	IntBinaryTreeIterator iterator(tree);
	while(iterator.has_next()) {
		printf("%d ", iterator.next());
	}
	printf("\n");

	printf("\nTesting some queues...");
	for(int n = 1; n < 128; n <<= 1) {
		long seed = time_ns();

		printf("\n");

		printf("STLQueue                 n=%.2d: ", n);
		int c1 = test_queue(new STLQueue<int>(), n, seed);

		printf("LLQueueSimple            n=%.2d: ", n);
		int c2 = test_queue(new LLQueueSimple<int>(), n, seed);

		printf("LLQueue                  n=%.2d: ", n);
		int c3 = test_queue(new LLQueue<int>(), n, seed);

		printf("ArrayQueue (static  s=n) n=%.2d: ", n);
		int c4 = test_queue(new ArrayQueue<int>(n, false), n, seed);

		printf("ArrayQueue (dynamic s=2) n=%.2d: ", n);
		int c5 = test_queue(new ArrayQueue<int>(2, true), n, seed);

		printf("VirtualArrayQueue        n=%.2d: ", n);
		int c6 = test_queue(new VirtualArrayQueue<int>(2, true), n, seed);

		/* Checksums should add together to zero */
		assert(c1 + c2 + c3 - c4 - c5 - c6 == 0);
	}

	return 0;
}