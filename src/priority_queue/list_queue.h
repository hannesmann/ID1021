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

	int push(int value) override {
		if(m_order == ListQueue_FastAdd) {
			return unordered_push(value);
		}
		else {
			return ordered_push(value);
		}
	}

	optional<PriorityQueueResult> pop() override {
		if(m_order == ListQueue_FastAdd) {
			return unordered_pop();
		}
		else {
			return ordered_pop();
		}
	}

	optional<PriorityQueueResult> increment(int offset) override {
		optional<PriorityQueueResult> result = pop();

		if(result) {
			return PriorityQueueResult { result.value().value, push(result.value().value + offset) };
		}

		return nullopt;
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
	int unordered_push(int value);
	optional<PriorityQueueResult> unordered_pop();

	int ordered_push(int value);
	optional<PriorityQueueResult> ordered_pop();

	LinkedList<int>* m_first = nullptr;
	LinkedList<int>* m_last = nullptr;

	ListQueueOrder m_order;
};