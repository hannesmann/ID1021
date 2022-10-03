#include "iterator.h"

IntBinaryTreeIterator::IntBinaryTreeIterator(const IntBinaryTree& tree) {
	fill_stack(tree.root);
	m_next = m_stack.pop().value_or(nullptr);
}

void IntBinaryTreeIterator::fill_stack(const IntBinaryTreeNode* source) {
	if(source) {
		if(source->right) {
			fill_stack(source->right);
		}

		m_stack.push(source);

		if(source->left) {
			fill_stack(source->left);
		}
	}
}

bool IntBinaryTreeIterator::has_next() const {
	return m_next != nullptr;
}

int IntBinaryTreeIterator::next() {
	if(m_next) {
		int value = m_next->value;
		m_next = m_stack.pop().value_or(nullptr);
		return value;
	}

	return -1;
}