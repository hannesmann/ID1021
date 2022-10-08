#pragma once

#include "linked_queue.h"
#include "tree.h"

class IntBinaryTreeIterator {
public:
	IntBinaryTreeIterator(const IntBinaryTree& tree);

	bool has_next() const;
	int next();
private:
	const IntBinaryTreeNode* m_next;
	LLQueue<const IntBinaryTreeNode*> m_queue;
};