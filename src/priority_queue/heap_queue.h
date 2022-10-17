#pragma once

#include "list.h"
#include "queue.h"

/* Forward declaration */
struct HeapQueueNode;

/* Holds subtree information for HeapQueueNode */
struct HeapQueueSubNode {
	/* The start of a subtree with a heap structure */
	HeapQueueNode* node = nullptr;
	/* How many nodes are present further down the tree */
	int subnodes = 0;
};

struct HeapQueueNode {
	/* Value of this node */
	int value = 0;
	/* Number of times this value is contained in the queue - this is necessary to be able to push the same value multiple times */
	int count = 1;

	HeapQueueSubNode left;
	HeapQueueSubNode right;

	HeapQueueNode(int value) : value(value) { }

	/* Add a value to the queue or increment its "reference counter" */
	void add_or_update(int value, int* depth);
	/* Bubble the current value to the top - returns false if this node will be destroyed */
	bool promote();
	/* Offset this value and balance the tree if applicable */
	void offset_existing(int offest, int* depth);
	/* Recursively deletes all subnodes (caller is responsible for deleting the node itself) */
	void delete_tree();
};

class HeapQueue : public PriorityQueue {
public:
	~HeapQueue() override {
		if(m_root) {
			m_root->delete_tree();
			delete m_root;
		}
	}

	int push(int value) override;
	optional<int> pop() override;
	optional<int> pop_and_increment(int offset, int* depth) override;

	const char* describe() override { return "HeapQueue"; }
private:
	HeapQueueNode* m_root = nullptr;;
};