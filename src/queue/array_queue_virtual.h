#pragma once

#include <assert.h>
#include "queue.h"

/* Implementation of array queue with a static or dynamic size, using mapping from an endless virtual address space to the real array */
template<typename T>
class VirtualArrayQueue : public Queue<T> {
public:
	VirtualArrayQueue(int slots, bool dynamic) :
		m_array(new T[slots]),
		m_slots(slots),
		m_dynamic(dynamic) { }

	~VirtualArrayQueue() override {
		delete[] m_array;
	}

	void push(T value) override {
		/* If we have written to all available slots, call the overflow handler */
		if(used_slots() == m_slots) {
			overflow_handler();
		}

		m_array[map(m_next_virtual_write_slot++)] = value;
	}

	optional<T> pop() override {
		if(used_slots()) {
			return m_array[map(m_next_virtual_read_slot++)];
		}

		return nullopt;
	}

private:
	/* Maps a virtual address to a real memory slot */
	inline int map(int virtual_position) {
		return virtual_position % m_slots;
	}

	/* Amount of slots occupied */
	inline int used_slots() {
		return m_next_virtual_write_slot - m_next_virtual_read_slot;
	}

	void overflow_handler() {
		/* Crashes if we're not allowed to grow the array */
		assert(m_dynamic);

		T* old_array = m_array;
		T* new_array = new T[m_slots * 2];

		for(int i = 0; i < m_slots; i++) {
			new_array[i] = m_array[map(m_next_virtual_read_slot + i)];
		}

		m_array = new_array;
		delete[] old_array;

		m_slots *= 2;

		m_next_virtual_write_slot -= m_next_virtual_read_slot;
		m_next_virtual_read_slot = 0;
	}

	/* Backing store for queue */
	T* m_array;
	/* Number of available slots */
	int m_slots;

	/* The next slot that will be read from when pop() is called */
	int m_next_virtual_read_slot = 0;
	/* The next slot that will be written to when push() is called */
	int m_next_virtual_write_slot = 0;

	/* If the array is allowed to grow on overflow */
	bool m_dynamic;
};