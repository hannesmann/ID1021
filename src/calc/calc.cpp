#include "calc.h"
#include <assert.h>

RPNCalculator::RPNCalculator(Item* expression, size_t count, bool dynamic_stack) {
	m_current_item = expression;
	m_end_of_expression = expression + count;

	if(dynamic_stack) {
		m_stack = std::make_unique<DynamicStack>();
	}
	else {
		/* Allocates theoretical maximum stack size (1 2 3 4 5 6 7 8 9... with no operations) */
		m_stack = std::make_unique<FixedStack>(count);
	}
}

int RPNCalculator::run(bool print_expression) {
	if(print_expression) {
		printf("  Calculating expression with %s: (", m_stack->describe());

		for(Item* item = m_current_item; item != m_end_of_expression; item++) {
			printf(" %s", item->to_text().c_str());
		}
		printf(" )\n");
	}

	while(m_current_item != m_end_of_expression) {
		step();
	}

	return m_stack->pop();
}

void RPNCalculator::step() {
	Item* item = m_current_item++;

	if(item->type == ITEM_VALUE) {
		m_stack->push(item->value);
	}
	else if(item->type == ITEM_MOD10) {
		m_stack->push(m_stack->pop() % 10);
	}
	else {
		int y = m_stack->pop();
		int x = m_stack->pop();

		switch(item->type) {
			case ITEM_ADD:
				m_stack->push(x + y);
				break;
			case ITEM_SUB:
				m_stack->push(x - y);
				break;

			case ITEM_MUL:
				m_stack->push(x * y);
				break;
			case ITEM_DIV:
				m_stack->push(x / y);
				break;

			case ITEM_MUL_OVERFLOW:
				int result = x * y;
				assert(x < 10 && y < 10);

				if(result > 9) {
					int first_digit = result % 10;
					int second_digit = (result / 10) % 10;
					result = first_digit + second_digit;
				}

				m_stack->push(result);
				break;
		}
	}
}