#include "heap_queue.h"

inline void swap(int& a, int& b) {
	int c = b;
	b = a;
	a = c;
}

int HeapQueueNode::add(int value) {
	HeapQueueSubNode* branch = (right.subnodes < left.subnodes) ? &right : &left;

	/* If the value is smaller than the root (this->value) then add the value of this node further down the tree */
	if(value < this->value) {
		swap(value, this->value);
	}

	branch->subnodes++;

	if(branch->node) {
		return 1 + branch->node->add(value);
	}
	else {
		branch->node = new HeapQueueNode(value);
		return 0;
	}
}

bool HeapQueueNode::promote() {
	bool promote_right =
		/* Condition A: We only have a single branch (or none) */
		(!left.subnodes && right.subnodes) ||
		/* Condition B: Select the smallest branch */
		(left.subnodes && right.subnodes && right.node->value < left.node->value);

	HeapQueueSubNode* branch_to_promote = promote_right ? &right : &left;

	if(branch_to_promote->subnodes) {
		value = branch_to_promote->node->value;

		if(!branch_to_promote->node->promote()) {
			delete branch_to_promote->node;

			branch_to_promote->node = nullptr;
			branch_to_promote->subnodes = 0;
		}

		return true;
	}

	return false;
}

int HeapQueueNode::offset(int n) {
	value += n;
	return balance();
}

int HeapQueueNode::balance() {
	HeapQueueSubNode* branch_to_swap_with = nullptr;

	/* If we have a left branch AND the value is less than the current node */
	if(left.subnodes &&
		left.node->value < value) {
		branch_to_swap_with = &left;
	}
	/* Check right branch too */
	else if(right.subnodes &&
		right.node->value < value) {
		branch_to_swap_with = &right;
	}

	if(branch_to_swap_with) {
		swap(value, branch_to_swap_with->node->value);
		return 1 + branch_to_swap_with->node->balance();
	}

	return 0;
}

void HeapQueueNode::destroy() {
	if(left.node) {
		left.node->destroy();
	}

	if(right.node) {
		right.node->destroy();
	}

	delete this;
}

int HeapQueue::push(int value) {
	if(m_root) {
		return m_root->add(value);
	}
	else {
		m_root = new HeapQueueNode(value);
		return 0;
	}
}

optional<PriorityQueueResult> HeapQueue::pop() {
	if(m_root) {
		int value = m_root->value;

		if(!m_root->promote()) {
			delete m_root;
			m_root = nullptr;
		}

		return PriorityQueueResult { value, 0 };
	}

	return nullopt;
}

optional<PriorityQueueResult> HeapQueue::increment(int offset) {
	if(m_root) {
		int value = m_root->value;
		return PriorityQueueResult { value, m_root->offset(offset) };
	}

	return nullopt;
}