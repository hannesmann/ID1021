#include <algorithm>
#include "heap_array_queue.h"

using std::swap;

int HeapArrayQueue::push(int value) {
	m_slots[++m_offset] = value;
	assert(m_offset < m_available_slots);

	int node = m_offset;
	int depth = 0;

	while(node) {
		depth++;
		int parent = map_parent_node(node);

		if(m_slots[parent] > m_slots[node]) {
			std::swap(m_slots[parent], m_slots[node]);
			node = parent;
		}
		else {
			node = 0;
		}
	}

	return depth;
}