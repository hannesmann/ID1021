#include <chrono>
#include <stdio.h>

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

int main(int argc, char** argv) {
	printf("Testing timer resolution...\n");

	long ntotal = 0;

	for(int i = 0; i < 10; i++) {
		long n0 = time_ns();
		long n1 = time_ns();

		ntotal += n1 - n0;
		printf("  i=%d:     %ld ns\n", i, n1 - n0);
	}

	printf("  Average: %ld ns\n", ntotal / 10);
}