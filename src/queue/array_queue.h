#pragma once

#include <assert.h>
#include "queue.h"

/* Implementation of array queue with a static or dynamic size */
template<typename T>
class ArrayQueue : public Queue<T> {
public:
	ArrayQueue(int slots, bool dynamic) :
		m_array(new T[slots]),
		m_slots(slots),
		m_dynamic(dynamic) { }

	~ArrayQueue() override {
		delete[] m_array;
	}

	void push(T value) override {
		/* If the next write slot is going to overwrite the next read slot, and the queue is not empty, we have overflowed */
		if(m_next_read_slot == m_next_write_slot && m_items_in_queue) {
			overflow_handler();
		}

		m_array[m_next_write_slot++] = value;
		m_next_write_slot = m_next_write_slot % m_slots;

		m_items_in_queue = true;
	}

	std::optional<T> pop() override {
		/* If there is no distance between the first and last slot, there are no items in the queue */
		if(m_items_in_queue) {
			T value = m_array[m_next_read_slot++];
			m_next_read_slot = m_next_read_slot % m_slots;

			/* If the next read slot has not been written yet, we have emptied the queue */
			if(m_next_read_slot == m_next_write_slot) {
				m_items_in_queue = false;
			}

			return value;
		}

		return std::nullopt;
	}

private:
	void overflow_handler() {
		/* Asserts if we're not allowed to grow the array */
		assert(m_dynamic);

		T* old_array = m_array;
		T* new_array = new T[m_slots * 2];

		int i = 0;
		/* Copy from next read slot to end */
		for(int j = m_next_read_slot; j < m_slots; j++) {
			new_array[i++] = old_array[j];
		}
		/* Copy remaining items that have wrapped around */
		for(int j = 0; j < m_next_read_slot; j++) {
			new_array[i++] = old_array[j];
		}

		m_array = new_array;
		delete[] old_array;

		m_next_read_slot = 0;
		m_next_write_slot = m_slots;

		m_slots *= 2;
	}

	/* Backing store for queue */
	T* m_array;
	/* Number of available slots */
	int m_slots;

	/* If queue is empty */
	bool m_items_in_queue = false;

	/* The next slot that will be read from when pop() is called */
	int m_next_read_slot = 0;
	/* The next slot that will be written to when push() is called */
	int m_next_write_slot = 0;

	/* If the array is allowed to grow on overflow */
	bool m_dynamic;
};