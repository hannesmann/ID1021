#include "queue.h"

#include <algorithm>
#include <assert.h>
#include <stdlib.h>

class HeapArrayQueue : public PriorityQueue {
public:
	HeapArrayQueue(int minimum_reserved = 16) :
		m_slots((int*)malloc(minimum_reserved * sizeof(int))),
		m_available_slots(minimum_reserved),
		m_minimum_slots(minimum_reserved) { }

	~HeapArrayQueue() override {
		free(m_slots);
	}

	int push(int value) override;
	optional<PriorityQueueResult> pop() override;
	optional<PriorityQueueResult> increment(int offset) override;

	const char* describe() override { return "HeapArrayQueue"; }
private:
	/* Bubble values from 0..start */
	int bubble(int start);
	/* Sink values from start..m_offset */
	int sink(int start);

	/* Find the parent of this node */
	inline int find_parent(int node) {
		/* Not a branch of any parent node */
		if(!node) {
			return 0;
		}
		/* If the node is located at an even location, this is a right branch (2n + 2) */
		else if(node % 2 == 0) {
			return (node - 2) / 2;
		}
		/* If the node is located at an odd location, this is a left branch (2n + 1) */
		else {
			return (node - 1) / 2;
		}
	}

	/* If this node has a left/right branch */
	inline bool has_branch(int node, bool right) {
		return m_offset >= find_branch(node, right);
	}
	/* Find the left and right branch of a node */
	inline int find_branch(int node, bool right) {
		return (node * 2) + 1 + right;
	}

	/* Items in queue */
	inline int size() {
		return m_offset + 1;
	}

	int* m_slots;

	/* Amount of available slots in m_slots */
	int m_available_slots;
	/* Minimum reserved size of m_slots */
	int m_minimum_slots;

	/* -1 == no root */
	int m_offset = -1;
};