#pragma once

#include "stack.h"
#include "tree.h"

class IntBinaryTreeIterator {
public:
	IntBinaryTreeIterator(const IntBinaryTree& tree);

	bool has_next() const;
	int next();
private:
	void walk_to_leftmost();

	const IntBinaryTreeNode* m_next;
	LinkedListStack<const IntBinaryTreeNode*> m_stack;
};