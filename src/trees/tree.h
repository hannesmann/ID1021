#pragma once

#include <stdio.h>

struct IntBinaryTreeNode {
	int key;
	int value;

	IntBinaryTreeNode* left = nullptr;
	IntBinaryTreeNode* right = nullptr;

	IntBinaryTreeNode(int key, int value) : key(key), value(value) { }

	void add_or_update(int key, int value);
	void destroy_tree();

	void print_tree(bool root) const;
};

struct IntBinaryTree {
	IntBinaryTreeNode* root = nullptr;
	~IntBinaryTree() { reset(); }

	const int* lookup(int key) const;
	void add(int key, int value);
	void reset();

	void print() const;
};