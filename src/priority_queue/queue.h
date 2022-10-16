#pragma once

#include <optional>

using std::optional;
using std::nullopt;

class PriorityQueue {
public:
	virtual ~PriorityQueue() {}

	/* Adds an item to the queue */
	virtual void push(int value) = 0;
	/* Removes the item with the highest priority (smaller integers are removed first) */
	virtual optional<int> pop() = 0;

	/* Name to show in logs */
	virtual const char* describe() = 0;
};