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

	HeapQueueSubNode left;
	HeapQueueSubNode right;

	HeapQueueNode(int value) : value(value) { }

	/* Add a value to the queue, returns depth traveled */
	int add(int value);
	/* Offset this value and balance the tree if applicable, returns depth traveled */
	int offset(int n);

	/* Bubble the current value to the top, returns false if this node will be destroyed */
	bool promote();
	/* Recursively balances this node with subnodes, returns depth traveled */
	int balance();

	/* Recursively deletes all subnodes and the node */
	void destroy();
};

class HeapQueue : public PriorityQueue {
public:
	~HeapQueue() override {
		if(m_root) {
			m_root->destroy();
		}
	}

	int push(int value) override;
	optional<PriorityQueueResult> pop() override;
	optional<PriorityQueueResult> increment(int offset) override;

	const char* describe() override { return "HeapQueue"; }
private:
	HeapQueueNode* m_root = nullptr;;
};