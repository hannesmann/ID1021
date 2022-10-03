/* From linked list assignment with std::optional for pop() */

#pragma once

#include <optional>
#include "list.h"

template<typename T>
class LinkedListStack {
public:
	LinkedListStack() : m_list(nullptr) {}

	~LinkedListStack() {
		LinkedList<T>* last = m_list;

		while(m_list) {
			last = m_list;
			m_list = m_list->tail;
			delete last;
		}
	}

	void push(T value) {
		LinkedList<T>* new_ll = new LinkedList<T>(value);

		if(m_list) {
			m_list->append(new_ll);
		}
		else {
			m_list = new_ll;
		}
	}

	std::optional<T> pop() {
		if(!m_list) {
			return std::nullopt;
		}

		LinkedList<T>* next_to_last = m_list;
		LinkedList<T>* last = m_list;

		while(last->tail) {
			next_to_last = last;
			last = last->tail;
		}

		T value = last->head;

		if(m_list->tail) {
			next_to_last->tail = nullptr;
		}
		else {
			m_list = nullptr;
		}

		delete[] last;
		return value;
	}
private:
	LinkedList<T>* m_list;
};