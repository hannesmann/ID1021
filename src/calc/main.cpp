#include <chrono>

#include "item.h"
#include "calc.h"

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

static int operations[] = { 10000, 50000, 100000, 200000, 300000, 400000, 500000 };

void benchmark_stack(std::vector<Item> expressions[7], bool dynamic) {
	static int repeats = 100;

	for(int i = 0; i < 7; i++) {
		long tstart = time_ns();

		for(int r = 0; r < repeats; r++) {
			RPNCalculator calculator(expressions[i].data(), expressions[i].size(), dynamic);
			calculator.run(false);
		}

		long tdiff = time_ns() - tstart;
		double t = tdiff / (double)repeats / 1000000.0;

		printf("  n=%d: %lf ms\n", operations[i], t);
	}
}

int main(int argc, char** argv) {
	srand(time_ns());

	printf("Testing RPN calculator...\n");
	for(int i = 0; i < 5; i++) {
		std::vector<Item> expression = generate_test_expression();

		RPNCalculator calculator(expression.data(), expression.size(), false);
		int result = calculator.run(true);
		printf("  Result: %d\n\n", result);
	}

	printf("\nBenchmarking stacks...\n");

	std::vector<Item> benchmark_expressions[7];
	for(int i = 0; i < 7; i++) {
		benchmark_expressions[i] = generate_benchmark_expression(operations[i]);
	}

	printf("  Static stack (allocated in constructor)\n");
	benchmark_stack(benchmark_expressions, false);
	printf("\n  Dynamic stack (allocated dynamically)\n");
	benchmark_stack(benchmark_expressions, true);

	const char* pn = "0101010346"; /* randomly generated */
	std::vector<Item> pn_expression;

	pn_expression.push_back(Item::number(10));

	for(int i = 0; i < 9; i++) {
		int digit = pn[i] - '0';
		pn_expression.push_back(Item::number(digit));

		if(i % 2 == 0) {
			pn_expression.push_back(Item::number(2));
			pn_expression.push_back(Item::operation(ITEM_MUL_OVERFLOW));
		}
	}

	for(int i = 0; i < 8; i++) {
		pn_expression.push_back(Item::operation(ITEM_ADD));
	}

	pn_expression.push_back(Item::operation(ITEM_MOD10));
	pn_expression.push_back(Item::operation(ITEM_SUB));

	printf("\nCalculating last digit of personal number %s...\n", pn);
	RPNCalculator calculator(pn_expression.data(), pn_expression.size(), false);
	int result = calculator.run(true);
	printf("  Last digit: %d\n\n", result);

	return 0;
}

