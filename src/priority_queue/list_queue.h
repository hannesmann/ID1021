#pragma once

#include "list.h"
#include "queue.h"

/* What the priority list queue should priorize, fast adding or fast removing */
enum ListQueueOrder {
	/* List is not implicitly ordered */
	ListQueue_FastAdd,
	/* List is implicitly ordered */
	ListQueue_FastRemove
};

class PriorityListQueue : public PriorityQueue {
public:
	PriorityListQueue(ListQueueOrder order) : m_order(order) { }

	~PriorityListQueue() override {
		if(m_first) delete m_first;
	}

	void push(int value) override {
		if(m_order == ListQueue_FastAdd) {
			unordered_push(value);
		}
		else {
			ordered_push(value);
		}
	}

	optional<int> pop() override {
		if(m_order == ListQueue_FastAdd) {
			return unordered_pop();
		}
		else {
			return ordered_pop();
		}
	}

	const char* describe() override {
		if(m_order == ListQueue_FastAdd) {
			return "PriorityListQueue O(1) Add";
		}
		else {
			return "PriorityListQueue O(1) Remove";
		}
	}
private:
	void unordered_push(int value);
	optional<int> unordered_pop();

	void ordered_push(int value);
	optional<int> ordered_pop();

	LinkedList<int>* m_first = nullptr;
	LinkedList<int>* m_last = nullptr;

	ListQueueOrder m_order;
};