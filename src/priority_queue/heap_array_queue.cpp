#include <algorithm>
#include <stdio.h>

#include "heap_array_queue.h"

using std::swap;

int HeapArrayQueue::push(int value) {
	if(m_offset + 1 >= m_available_slots) {
		m_available_slots = std::max(m_available_slots * 2, 1);
		m_slots = (int*)realloc(m_slots, m_available_slots * sizeof(int));
	}

	m_slots[++m_offset] = value;
	return bubble(m_offset);
}

optional<PriorityQueueResult> HeapArrayQueue::pop() {
	if(!size()) {
		return nullopt;
	}

	int value = m_slots[0];
	m_slots[0] = m_slots[m_offset--];

	if(std::max(m_offset, m_minimum_slots - 1) < m_available_slots / 2) {
		m_available_slots /= 2;
		m_slots = (int*)realloc(m_slots, m_available_slots * sizeof(int));
	}

	return PriorityQueueResult { value, sink(0) };
}

optional<PriorityQueueResult> HeapArrayQueue::increment(int offset) {
	if(!size()) {
		return nullopt;
	}

	int value = m_slots[0];
	m_slots[0] += offset;

	return PriorityQueueResult { value, sink(0) };
}

int HeapArrayQueue::bubble(int start) {
	int parent = find_parent(start);

	if(m_slots[parent] > m_slots[start]) {
		swap(m_slots[parent], m_slots[start]);
		return 1 + bubble(parent);
	}

	return 0;
}

int HeapArrayQueue::sink(int start) {
	/* If the queue has <n items, we may accidentally read an invalid array location */
	if(m_offset < start) {
		return 0;
	}

	bool has_left = has_branch(start, false);
	bool has_right = has_branch(start, true);

	int left = find_branch(start, false);
	int right = find_branch(start, true);

	/* Condition A: Select left if available, otherwise select right if available, otherwise compare with self (always false) */
	int sink_to = has_left ? left : (has_right ? right : start);

	/* Condition B: If we have both left and right branches, sink to the smaller of the two */
	if(has_left && has_right) {
		sink_to = m_slots[right] < m_slots[left] ? right : left;
	}

	if(m_slots[sink_to] < m_slots[start]) {
		swap(m_slots[sink_to], m_slots[start]);
		return 1 + sink(sink_to);
	}

	return 0;
}