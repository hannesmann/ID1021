/* Copy of stack from the second assignment */

#pragma once

#include "list.h"

class LinkedListStack {
public:
	LinkedListStack() : m_list(nullptr) {}
	~LinkedListStack();

	void push(int value);
 	int pop();
private:
	LinkedList<int>* m_list;
};