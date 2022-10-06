#include "iterator.h"

void IntBinaryTreeIterator::walk_to_leftmost() {
	while(m_next && m_next->left) {
		m_stack.push(m_next);
		m_next = m_next->left;
	}
}

IntBinaryTreeIterator::IntBinaryTreeIterator(const IntBinaryTree& tree) {
	m_next = tree.root;
	/* Start by walking to the leftmost node */
	walk_to_leftmost();
}

bool IntBinaryTreeIterator::has_next() const {
	return m_next != nullptr;
}

int IntBinaryTreeIterator::next() {
	if(m_next) {
		int value = m_next->value;

		/* Unexplored path */
		if(m_next->right) {
			/* Important not to push the same node on the stack again - this causes an infinite loop */
			m_next = m_next->right;
			walk_to_leftmost();
		}
		/* Walk back towards the root node */
		else {
			m_next = m_stack.pop().value_or(nullptr);
		}

		return value;
	}

	return -1;
}