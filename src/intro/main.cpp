#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

/* Measures the time it takes for a random access in an array of size n */
double random_access(int n) {
	/* Number of times to repeat operation */
	static int rounds = 10000;
	/* Number of times to search array */
	int iterations = n;

	/* Positions to search in array */
	int* indices = new int[iterations];
	for(int i = 0; i < iterations; i++) {
		indices[i] = rand() % n;
	}

	/* The array containing dummy values */
	int* array = new int[n];
	memset(array, -1, n);

	long start_time = time_ns();
	for(int round = 0; round < rounds; round++) {
		for(int i = 0; i < iterations; i++) {
			int dummy = array[indices[i]];
		}
	}
	long diff_access = time_ns() - start_time;

	start_time = time_ns();
	for(int round = 0; round < rounds; round++) {
	}
	long diff_dummy = time_ns() - start_time;

	double time = (diff_access - diff_dummy) / ((double)rounds * (double)iterations);
	delete[] array;
	delete[] indices;

	return time;
}

/* Measures the time it takes to search through array of size n for a random key */
double search(int n, int keys) {
	/* Number of times to repeat operation */
	static int rounds = 100;

	/* Keys go from 0 to n * 10 */
	int max_key_value = n * 10;
	int* random_keys = new int[keys];
	int* array = new int[n];

	long start_time = time_ns();

	for(int round = 0; round < rounds; round++) {
		for(int i = 0; i < keys; i++) {
			random_keys[i] = rand() % (max_key_value + 1);
		}

		for(int i = 0; i < n; i++) {
			array[n] = rand() % (max_key_value + 1);
		}

		for(int k = 0; k < keys; k++) {
			int test_key = random_keys[k];

			for(int i = 0; i < n; i++) {
				if(array[i] == test_key) {
					break;
				}
			}
		}
	}

	double time = (time_ns() - start_time) / (double)rounds;
	delete[] array;
	delete[] random_keys;

	return time;
}

int main(int argc, char** argv) {
	printf("Testing timer resolution...\n");

	long ntotal = 0;

	for(int i = 0; i < 10; i++) {
		long n0 = time_ns();
		long n1 = time_ns();

		ntotal += n1 - n0;
		printf("  i=%d: %ld ns\n", i, n1 - n0);
	}

	printf("  Average: %ld ns\n", ntotal / 10);

	static int sizes[] = { 100, 500, 1000, 2000, 3000, 4000, 5000 };
	srand(time_ns());

	printf("\nTesting random access in array of size n...\n");
	for(unsigned long i = 0; i < sizeof(sizes) / sizeof(int); i++) {
		double ns = random_access(sizes[i]);
		printf("  n=%d: %lf ns\n", sizes[i], ns);
	}

	printf("\nTesting search (m=100) in an array...\n");
	for(unsigned long i = 0; i < sizeof(sizes) / sizeof(int); i++) {
		double ns = search(sizes[i], 100);
		printf("  n=%d: %lf ns\n", sizes[i], ns);
	}

	printf("\nTesting search (m=n) for duplicates in an array...\n");
	for(unsigned long i = 0; i < sizeof(sizes) / sizeof(int); i++) {
		double ns = search(sizes[i], sizes[i]);
		printf("  n=%d: %lf ns\n", sizes[i], ns);
	}
}