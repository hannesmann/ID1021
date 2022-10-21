#include <chrono>

#include <assert.h>
#include <functional>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ziptable.h"
#include "hashtable.h"

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
			printf("Check " #expr " failed: Expected %d, got %d\n", expected, result); \
			exit(EXIT_FAILURE); \
		} \
	}

int check_and_print_collisions(ZipCodeTable& table) {
	int best = table.find_best_mod(10000, 20000);
	static int mod[] = { 10000, 12500, 15000, 17500, 20000, 25000, 50000, MAX_ZIP, best };

	for(unsigned long i = 0; i < sizeof(mod) / sizeof(int); i++) {
		int n = mod[i];

		int* collisions = table.collisions(n);

		printf("\nCollisions for modulo %d:\n", n);
		for(int j = 0; j < MAX_COLLISIONS; j++) {
			if(!collisions[j]) {
				break;
			}

			printf("  %d key(s) map to the same index: %d\n", j + 1, collisions[j]);
		}

		delete[] collisions;
	}

	printf("\nBest for this table in range 10000-99999: mod %d\n", best);

	return best;
}

void test_hashtable(ZipCodeTable& codetable, ZipHashTableBase& hashtable) {
	verify(hashtable.has(12135), 0);

	hashtable.add(codetable.lookup_fast(12135));
	hashtable.add(codetable.lookup_fast(27630));
	hashtable.add(codetable.lookup_fast(59335));

	verify(hashtable.has(12135), 1);
	verify(hashtable.has(27630), 1);
	verify(hashtable.has(59335), 1);
	verify(hashtable.has(59336), 0);

	hashtable.remove(12135);

	verify(hashtable.has(12135), 0);
	verify(hashtable.has(27630), 1);
}

#define REPEATS 100

/* Receives a function F(str). */
void benchmark_lookup_str(const char* name, const char* key, std::function<void(const char*)> f) {
	double sum = 0;
	double min = 0;
	double max = 0;

	for(int i = 1; i <= 4; i++) {
		long tstart = time_ns();

		for(int repeat = 0; repeat < REPEATS; repeat++) {
			f(key);
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
	printf("  [%s] key='%s': %.2lf ns (± %.2lf ns)\n", name, key, avg / REPEATS, ((max - min) / 2.0) / REPEATS);
}

void benchmark_lookup_int(const char* name, int key, std::function<void(int)> f) {
	double sum = 0;
	double min = 0;
	double max = 0;

	for(int i = 1; i <= 4; i++) {
		long tstart = time_ns();

		for(int repeat = 0; repeat < REPEATS; repeat++) {
			f(key);
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
	printf("  [%s] key=%d: %.2lf ns (± %.2lf ns)\n", name, key, avg / REPEATS, ((max - min) / 2.0) / REPEATS);
}

int main(int argc, char** argv) {
	srand(time_ns());

	if(argc < 2) {
		printf("Please specify zipcodes.csv file to read from.");
		return EXIT_FAILURE;
	}

	ZipCodeTable table;
	if(!table.update_from_file(argv[1])) {
		printf("Read failed from zipcodes.csv ('%s').", argv[1]);
		return EXIT_FAILURE;
	}

	printf("Find '171 71': %s\n", table.lookup_linear("171 71") ? "yes" : "no");
	printf("Find '171 71' (binary): %s\n", table.lookup_binary("171 71") ? "yes" : "no");
	printf("Find 17171: %s\n", table.lookup_linear(17171) ? "yes" : "no");
	printf("Find 17171 (fast): %s\n", table.lookup_fast(17171) ? "yes" : "no");

	int present = 0;
	for(int i = 0; i < MAX_ZIP; i++) {
		present += !!table.zips()[i];
	}

	printf("Array filled: %.2lf%%\n", present / (double)MAX_ZIP * 100);

	int best = check_and_print_collisions(table);

	printf("\nTesting hashtables...\n");

	printf("  ZipHashtable... ");
	{
		ZipHashTable hashtable(best);
		test_hashtable(table, hashtable);
	}
	printf("Correct\n");

	printf("  ZipArrayHashTable... ");
	{
		ZipArrayHashTable hashtable(best, 1);
		test_hashtable(table, hashtable);
	}
	printf("Correct\n");

	printf("\nBenchmarking lookup...\n");

	benchmark_lookup_str("lookup_linear_str", "111 15", [&table] (const char* key) { table.lookup_linear(key); });
	benchmark_lookup_str("lookup_linear_str", "994 99", [&table] (const char* key) { table.lookup_linear(key); });
	benchmark_lookup_str("lookup_binary_str", "111 15", [&table] (const char* key) { table.lookup_binary(key); });
	benchmark_lookup_str("lookup_binary_str", "994 99", [&table] (const char* key) { table.lookup_binary(key); });
	benchmark_lookup_int("lookup_linear_int", 11115, [&table] (int key) { table.lookup_linear(key); });
	benchmark_lookup_int("lookup_linear_int", 99499, [&table] (int key) { table.lookup_linear(key); });
	benchmark_lookup_int("lookup_binary_int", 11115, [&table] (int key) { table.lookup_binary(key); });
	benchmark_lookup_int("lookup_binary_int", 99499, [&table] (int key) { table.lookup_binary(key); });
	benchmark_lookup_int("lookup_fast_int", 11115, [&table] (int key) { table.lookup_fast(key); });
	benchmark_lookup_int("lookup_fast_int", 99499, [&table] (int key) { table.lookup_fast(key); });

	printf("\nBenchmarking ZipArrayHashTable depth...\n");

	for(int n = 0; n < 5; n++) {
		ZipArrayHashTable hashtable(best, n);
		for(int i = 11111; i < 44444; i++) {
			if(table.lookup_fast(i)) {
				hashtable.add(table.lookup_fast(i));
			}
		}

		/* Intentionally picked to cause collision */
		hashtable.get(44395);
		printf("  Depth for expected_duplicates=%d (array_size=%d): %d\n", n, best * (n + 1), hashtable.last_depth);
	}

	return 0;
}