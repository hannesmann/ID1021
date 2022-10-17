#include "queue.h"

#include <algorithm>
#include <assert.h>

class HeapArrayQueue : public PriorityQueue {
public:
	HeapArrayQueue(int n) :
		m_slots(new int[n]),
		m_available_slots(n) { }

	~HeapArrayQueue() override {
		delete[] m_slots;
	}

	int push(int value) override;
	optional<int> pop();
	optional<int> pop_and_increment(int offset, int* depth) override;

	const char* describe() override { return "HeapArrayQueue"; }
private:
	inline int map_parent_node(int node) {
		if(node % 2) {
			return (node - 2) / 2;
		}
		else {
			return (node - 1) / 2;
		}
	}

	inline int map_branch_node(int node, bool right) {
		return (node * 2) + 1 + right;
	}

	int* m_slots;
	int m_available_slots;

	/* -1 == no root */
	int m_offset = -1;
};