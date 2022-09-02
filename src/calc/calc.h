/* calc.h - Implementation of RPN calculator */
#pragma once

#include "item.h"
#include "stack.h"
#include <memory>

class RPNCalculator {
public:
	RPNCalculator(Item* expression, size_t count, bool dynamic_stack);
	int run();
private:
	void step();

	Item* m_current_item;
	/* Pointer past the last item */
	Item* m_end_of_expression;

	std::unique_ptr<Stack> m_stack;
};