#pragma once

#include "queue.h"

/* Simple implementation of linked list queue with only pointer to first element */
template<typename T>
class LLQueueSimple : public Queue<T> {
public:
	~LLQueueSimple() override {
		if(m_list) delete m_list;
	}

	void push(T value) override {
		LinkedList<T>* last = m_list->last();

		if(last) {
			last->next = new LinkedList<T>(value);
		}
		else {
			m_list = new LinkedList<T>(value);
		}
	}

	std::optional<T> pop() override {
		std::optional<T> value = std::nullopt;

		if(m_list) {
			LinkedList<T>* first = m_list;
			value = first->item;
			m_list = first->next;

			first->next = nullptr;
			delete first;
		}

		return value;
	}
private:
	LinkedList<T>* m_list = nullptr;
};

/* Implementation of linked list queue with pointer to first and last element */
template<typename T>
class LLQueue : public Queue<T> {
public:
	~LLQueue() override {
		if(m_first) delete m_first;
	}

	void push(T value) override {
		if(m_last) {
			m_last = m_last->next = new LinkedList<T>(value);
		}
		else {
			m_first = m_last = new LinkedList<T>(value);
		}
	}

	std::optional<T> pop() override {
		std::optional<T> value = std::nullopt;

		if(m_first) {
			LinkedList<T>* first = m_first;
			value = first->item;
			m_first = first->next;

			first->next = nullptr;
			delete first;
		}

		if(!m_first) {
			m_last = nullptr;
		}

		return value;
	}
private:
	LinkedList<T>* m_first = nullptr;
	LinkedList<T>* m_last = nullptr;
};