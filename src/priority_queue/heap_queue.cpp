#include <algorithm>
#include "heap_queue.h"

using std::swap;

void HeapQueueNode::add_or_increment(int value) {
	if(value == this->value) {
		this->count++;
	}
	else {
		HeapQueueSubNode* branch = (right.subnodes < left.subnodes) ? &right : &left;

		/* TODO */
		if(value < this->value) {
			swap(value, this->value);
		}

		if(branch->node) {
			branch->node->add_or_increment(value);
		}
		else {
			branch->node = new HeapQueueNode(value);
		}

		branch->subnodes++;
	}
}

void HeapQueueNode::delete_tree() {
	if(left.node) {
		left.node->delete_tree();
		left.subnodes--;

		delete left.node;
	}

	if(right.node) {
		right.node->delete_tree();
		right.subnodes--;

		delete right.node;
	}

	left.node = right.node = nullptr;
}

void HeapQueue::push(int value) {
	if(m_root) {
		m_root->add_or_increment(value);
	}
	else {
		m_root = new HeapQueueNode(value);
	}
}

optional<int> HeapQueue::pop() {
	if(m_root) {
		if(m_root->count > 1) {
			m_root->count--;
		}
		else {

		}

		return m_root->value;
	}

	return nullopt;
}