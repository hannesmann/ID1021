#include <chrono>

#include "item.h"
#include "calc.h"

using namespace std::chrono;

long time_ns() {
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}

int main(int argc, char** argv) {
	static Item expression[] = {
		Item::number(3),
		Item::number(5),
		Item::operation(ITEM_SUB)
	};

	RPNCalculator calc(expression, sizeof(expression) / sizeof(Item), true);
	int result = calc.run();
	printf("Result: %d\n", result);

	return 0;
}