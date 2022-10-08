#include "iterator.h"

IntBinaryTreeIterator::IntBinaryTreeIterator(const IntBinaryTree& tree) {
	m_next = tree.root;
}

bool IntBinaryTreeIterator::has_next() const {
	return m_next != nullptr;
}

int IntBinaryTreeIterator::next() {
	if(m_next) {
		int value = m_next->value;

		if(m_next->left) {
			m_queue.push(m_next->left);
		}
		if(m_next->right) {
			m_queue.push(m_next->right);
		}
		m_next = m_queue.pop().value_or(nullptr);

		return value;
	}

	return -1;
}