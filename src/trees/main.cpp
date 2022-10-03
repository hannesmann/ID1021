#include <chrono>
#include <memory>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iterator.h"
#include "tree.h"

/* Ensure we have good random number quality */
static_assert(RAND_MAX == INT_MAX);

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

#define REPEATS 300

void benchmark_tree_lookup() {
	static int elemcount[] = { 5, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 250000 };

	for(unsigned long i = 0; i < sizeof(elemcount) / sizeof(int); i++) {
		int n = elemcount[i];

		double min = 0;
		double max = 0;

		for(int i = 1; i <= 8; i++) {
			IntBinaryTree tree;
			int keys[n];

			for(int j = 0; j < n; j++) {
				keys[j] = rand() % (100 * n);
				tree.add(keys[j], rand() % 10000);
			}

			long tstart = time_ns();
			for(int repeat = 0; repeat < REPEATS; repeat++) {
				int key = keys[rand() % n];
				tree.lookup(key);
			}
			long tdiff = time_ns() - tstart;

			/* First four rounds are warmup */
			if(i >= 4) {
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
	}
}

int main(int argc, char** argv) {
	srand(time_ns());

	printf("Benchmarking lookup() with binary tree...\n");
	benchmark_tree_lookup();

	printf("\nTesting IntBinaryTree::print()...\n");

	IntBinaryTree tree;
	for(int i = 0; i < 16; i++) {
		tree.add(rand() % 50, rand() % 50);
	}
	tree.print();

	printf("\nTesting IntBinaryTreeIterator...\n");
	tree.reset();
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

	return 0;
}