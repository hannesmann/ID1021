#pragma once

#include <optional>

using std::optional;
using std::nullopt;
using std::make_optional;

/* Contains the value and benchmark statistics for priority queue */
struct PriorityQueueResult {
	/* Value returned from the queue */
	int value;
	/* The depth required to travel down the tree, if applicable */
	int depth;
};

class PriorityQueue {
public:
	virtual ~PriorityQueue() {}

	/* Adds an item to the queue - returns depth traveled down the tree if applicable */
	virtual int push(int value) = 0;
	/* Removes the item with the highest priority (smaller integers are removed first) */
	virtual optional<PriorityQueueResult> pop() = 0;
	/* Removes the item with the highest priority and then adds it back with specified offest */
	virtual optional<PriorityQueueResult> increment(int offset) = 0;

	/* Name to show in logs */
	virtual const char* describe() = 0;
};