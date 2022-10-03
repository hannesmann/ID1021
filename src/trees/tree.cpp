#include "tree.h"

void IntBinaryTreeNode::add_or_update(int key, int value) {
	if(key == this->key) {
		this->value = value;
	}
	else if(key < this->key) {
		if(left) {
			left->add_or_update(key, value);
		}
		else {
			left = new IntBinaryTreeNode(key, value);
		}
	}
	else {
		if(right) {
			right->add_or_update(key, value);
		}
		else {
			right = new IntBinaryTreeNode(key, value);
		}
	}
}

void IntBinaryTreeNode::destroy_tree() {
	if(left) {
		left->destroy_tree();
		delete left;
		left = nullptr;
	}

	if(right) {
		right->destroy_tree();
		delete right;
		right = nullptr;
	}
}

void IntBinaryTreeNode::print_tree(bool root) const {
	if(left) {
		left->print_tree(false);
	}

	printf("%s%d:%d ", root ? "(root) " : "", key, value);

	if(right) {
		right->print_tree(false);
	}
}

const int* IntBinaryTree::lookup(int key) const {
	IntBinaryTreeNode* node = root;

	while(node) {
		if(node->key == key) {
			return &node->value;
		}

		node = (node->key < key) ? node->left : node->right;
	}

	return nullptr;
}

void IntBinaryTree::add(int key, int value) {
	if(root) {
		root->add_or_update(key, value);
	}
	else {
		root = new IntBinaryTreeNode(key, value);
	}
}

void IntBinaryTree::reset() {
	if(root) {
		root->destroy_tree();
		delete root;
		root = nullptr;
	}
}

void IntBinaryTree::print() const {
	if(root) {
		root->print_tree(true);
	}
	else {
		printf("(null)");
	}

	printf("\n");
}
