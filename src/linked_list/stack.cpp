#include <assert.h>
#include "stack.h"

void LinkedListStack::push(int value) {
	LinkedList<int>* new_ll = new LinkedList<int>(value);

	if(m_list) {
		m_list->append(new_ll);
	}
	else {
		m_list = new_ll;
	}
}

int LinkedListStack::pop() {
	assert(m_list);

	LinkedList<int>* next_to_last = m_list;
	LinkedList<int>* last = m_list;

	while(last->tail) {
		next_to_last = last;
		last = last->tail;
	}

	int value = last->head;

	if(m_list->tail) {
		next_to_last->tail = nullptr;
	}
	else {
		m_list = nullptr;
	}

	delete[] last;
	return value;
}

LinkedListStack::~LinkedListStack() {
	LinkedList<int>* last = m_list;

	while(m_list) {
		last = m_list;
		m_list = m_list->tail;
		delete last;
	}
}