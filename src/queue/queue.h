#pragma once

#include <optional>
#include <queue>
#include "list.h"

template<typename T>
class Queue {
public:
	virtual ~Queue() {}

	virtual void push(T value) = 0;
	virtual std::optional<T> pop() = 0;
};

/* Known good reference implementation */
template<typename T>
class STLQueue : public Queue<T> {
public:
	void push(T value) override {
		m_stl_queue.push(value);
	}

	std::optional<T> pop() override {
		if(m_stl_queue.size()) {
			T value = m_stl_queue.front();
			m_stl_queue.pop();
			return value;
		}

		return std::nullopt;
	}
private:
	std::queue<T> m_stl_queue;
};