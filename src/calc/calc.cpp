#include "calc.h"

RPNCalculator::RPNCalculator(Item* expression, size_t count, bool dynamic_stack) {
	m_current_item = expression;
	m_end_of_expression = expression + count;

	if(dynamic_stack) {
		m_stack = std::make_unique<DynamicStack>();
	}
	else {
		m_stack = std::make_unique<FixedStack>(4);
	}
}

int RPNCalculator::run() {
#ifdef DEBUG_PRINT
	printf("Calculating expression with %s: (", m_stack->describe());

	for(Item* item = m_current_item; item != m_end_of_expression; item++) {
		printf(" %s", item->to_text().c_str());
	}
	printf(" )\n");
#endif

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
		}
	}
}