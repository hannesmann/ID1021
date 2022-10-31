#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <chrono>
#include <functional>

#include "graph.h"

#include "methods/naive.h"
#include "methods/paths.h"

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

#define REPEATS 100

/* Receives a function optional<d> = F(). */
void benchmark_shortest_distance(const char* s, std::function<optional<int>()> f) {
	double sum = 0;
	bool found = false;

	for(int i = 1; i <= 4; i++) {
		long tstart = time_ns();

		for(int repeat = 0; repeat < REPEATS; repeat++) {
			found = found || f().has_value();
		}

		long tdiff = time_ns() - tstart;
		sum += tdiff;
	}

	double avg = sum / 4.0;
	printf("  [%s, found: %s] %.2lf ns\n", s, found ? "yes" : "no", avg / REPEATS);
}

struct SearchPath {
	const char* from;
	const char* to;
};

int main(int argc, char** argv) {
	srand(time_ns());

	if(argc < 2) {
		printf("Please specify trains.csv file to read from.");
		return EXIT_FAILURE;
	}

	Map map;
	if(!map.update_from_file(argv[1])) {
		printf("Read failed from trains.csv ('%s').", argv[1]);
		return EXIT_FAILURE;
	}

	{
		vector<City*> cities = map.table().all();

		for(City* city : cities) {
			printf("City '%s' with %lu connections\n", city->name, city->connections.size());

			for(Connection& connection : city->connections) {
				printf("  %d minutes to '%s'\n", connection.distance, connection.city->name);
			}
		}
	}

	SearchPath paths[] = {
		{"Malmö", "Göteborg"},
		{"Göteborg", "Stockholm"},
		{"Malmö", "Stockholm"},
		{"Stockholm", "Sundsvall"},
		{"Stockholm", "Umeå"},
		{"Göteborg", "Sundsvall"},
		{"Sundsvall", "Umeå"},
		{"Umeå", "Göteborg"},
		{"Göteborg", "Umeå"},
		{"Malmö", "Kiruna"}
	};

	printf("\nBenchmarking 'naive' method...\n");
	for(SearchPath& path : paths) {
		benchmark_shortest_distance((std::string(path.from) + " " + std::string(path.to)).c_str(), [&map, &path]() {
			return find_shortest_path_naive({map.table().get(path.from), map.table().get(path.to)}, 350);
		});
	}

	printf("\nBenchmarking 'paths' method (max: 350)...\n");
	for(SearchPath& path : paths) {
		benchmark_shortest_distance((std::string(path.from) + " " + std::string(path.to)).c_str(), [&map, &path]() {
			return find_shortest_path({map.table().get(path.from), map.table().get(path.to)}, 350);
		});
	}

	printf("\nBenchmarking 'paths' method (max: 10000)...\n");
	for(SearchPath& path : paths) {
		benchmark_shortest_distance((std::string(path.from) + " " + std::string(path.to)).c_str(), [&map, &path]() {
			return find_shortest_path({map.table().get(path.from), map.table().get(path.to)}, 10000);
		});
	}

	printf("\nBenchmarking 'enhanced paths' method...\n");
	for(SearchPath& path : paths) {
		benchmark_shortest_distance((std::string(path.from) + " " + std::string(path.to)).c_str(), [&map, &path]() {
			return find_shortest_path_enhanced({map.table().get(path.from), map.table().get(path.to)});
		});
	}

	const char* malmo_to_city[] = {
		"Lund",
		"Hässleholm",
		"Alvesta",
		"Linköping",
		"Uppsala",
		"Gävle",
		"Sundsvall",
		"Umeå",
		"Luleå",
		"Kiruna"
	};

	printf("\nHow long does it take to find a path from Malmö to:\n");
	for(const char* city : malmo_to_city) {
		long tstart = time_ns();
		int distance = find_shortest_path_enhanced({map.table().get("Malmö"), map.table().get(city)}).value_or(-1);
		long tdiff = time_ns() - tstart;

		printf("  %s: %ld ns (%d min)\n", city, tdiff, distance);
	}

	return 0;
}