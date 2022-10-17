#include <algorithm>
#include "heap_queue.h"

using std::swap;

void HeapQueueNode::add_or_update(int value, int* depth) {
	if(value == this->value) {
		this->count++;
	}
	else {
		HeapQueueSubNode* branch = (right.subnodes < left.subnodes) ? &right : &left;

		if(value < this->value) {
			swap(value, this->value);
		}

		if(branch->node) {
			branch->node->add_or_update(value, depth);
		}
		else {
			branch->node = new HeapQueueNode(value);
		}

		branch->subnodes++;
	}

	if(depth) {
		(*depth)++;
	}
}

bool HeapQueueNode::promote() {
	bool promote_left =
		/* Condition A: We only have a single branch (or none) */
		(left.subnodes && !right.subnodes) ||
		/* Condition B: Select the smallest branch */
		(left.subnodes && right.subnodes && left.node->value < right.node->value);

	HeapQueueSubNode* branch_to_promote = promote_left ? &left : &right;

	if(branch_to_promote && branch_to_promote->node) {
		value = branch_to_promote->node->value;
		count = branch_to_promote->node->count;

		if(!branch_to_promote->node->promote()) {
			delete branch_to_promote->node;

			branch_to_promote->node = nullptr;
			branch_to_promote->subnodes = 0;
		}

		return true;
	}

	return false;
}

void HeapQueueNode::offset_existing(int offset, int* depth) {
	/* Slow path */
	if(count > 1) {
		add_or_update(value + offset, depth);
		count--;
	}
	/* Fast path */
	else {
		value += offset;

		HeapQueueNode* current_node = this;
		while(true) {
			HeapQueueSubNode* branch_to_swap_with = nullptr;

			if(current_node->left.subnodes &&
				current_node->left.node->value < current_node->value) {
				branch_to_swap_with = &current_node->left;
			}
			else if(current_node->right.subnodes &&
				current_node->right.node->value < current_node->value) {
				branch_to_swap_with = &current_node->right;
			}

			if(branch_to_swap_with) {
				if(depth) {
					(*depth)++;
				}

				swap(current_node->value, branch_to_swap_with->node->value);
				swap(current_node->count, branch_to_swap_with->node->count);

				current_node = branch_to_swap_with->node;
			}
			else {
				break;
			}
		}
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

int HeapQueue::push(int value) {
	int depth = 0;

	if(m_root) {
		m_root->add_or_update(value, &depth);
	}
	else {
		m_root = new HeapQueueNode(value);
		depth++;
	}

	return depth;
}

optional<int> HeapQueue::pop() {
	if(m_root) {
		int value = m_root->value;

		if(m_root->count > 1) {
			m_root->count--;
		}
		else if(!m_root->promote()) {
			delete m_root;
			m_root = nullptr;
		}

		return value;
	}

	return nullopt;
}

optional<int> HeapQueue::pop_and_increment(int offset, int* depth) {
	if(depth) {
		*depth = 0;
	}

	if(m_root) {
		int value = m_root->value;
		m_root->offset_existing(offset, depth);
		return value;
	}

	return nullopt;
}