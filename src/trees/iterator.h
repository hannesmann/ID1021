#pragma once

#include "stack.h"
#include "tree.h"

class IntBinaryTreeIterator {
public:
	IntBinaryTreeIterator(const IntBinaryTree& tree);

	bool has_next() const;
	int next();
private:
	void fill_stack(const IntBinaryTreeNode* source);

	const IntBinaryTreeNode* m_next;
	LinkedListStack<const IntBinaryTreeNode*> m_stack;
};