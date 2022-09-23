#pragma once

#include <memory>
#include <optional>

template<typename T>
class LinkedList {
public:
	/* Construct list without tail */
	LinkedList(T item) {
		head = item;
		tail = nullptr;
	}

	/* Construct list with tail */
	LinkedList(T item, LinkedList<T>* tail) {
		head = item;
		this->tail = tail;
	}

	T head;
	LinkedList<T>* tail;

	/* Iterates through list to find last element */
	LinkedList<T>* last() {
		LinkedList<T>* next = this;

		while(next->tail) {
			next = next->tail;
		}

		return next;
	}

	/* Append element to end of list */
	void append(LinkedList<T>* list) {
		last()->tail = list;
	}
};