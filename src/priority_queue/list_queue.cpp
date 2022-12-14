#include "list_queue.h"

/* O(1) */
int PriorityListQueue::unordered_push(int value) {
	if(m_last) {
		m_last = m_last->next = new LinkedList<int>(value);
	}
	else {
		m_first = m_last = new LinkedList<int>(value);
	}

	return 0;
}

/* O(n) */
optional<PriorityQueueResult> PriorityListQueue::unordered_pop() {
	if(!m_first) {
		return nullopt;
	}

	int depth = 0;

	/* Keep track of current element in iteration */
	LinkedList<int>* next_to_current = nullptr;
	LinkedList<int>* current = m_first;

	/* Keep track of smallest item in iteration */
	LinkedList<int>* next_to_smallest = nullptr;
	LinkedList<int>* smallest = m_first;

	while(current) {
		depth++;

		if(current->item < smallest->item) {
			smallest = current;
			next_to_smallest = next_to_current;
		}

		next_to_current = current;
		current = current->next;
	}

	int value = smallest->item;

	/* Move first and last pointers forward before deleting anything */
	if(smallest == m_first)
		m_first = m_first->next;
	if(smallest == m_last)
		m_last = next_to_smallest;

	if(next_to_smallest)
		next_to_smallest->next = smallest->next;

	smallest->next = nullptr;
	delete smallest;

	return PriorityQueueResult { value, depth };
}

/* O(n) */
int PriorityListQueue::ordered_push(int value) {
	int depth = 0;

	if(m_first) {
		LinkedList<int>* new_node = new LinkedList<int>(value);

		LinkedList<int>* next_to_current = nullptr;
		LinkedList<int>* current = m_first;

		while(current) {
			/* If the current item is bigger, add the new node before it */
			if(current->item >= value) {
				if(next_to_current) {
					next_to_current->next = new_node;
				}
				else {
					m_first = new_node;
				}

				new_node->next = current;
				return depth;
			}

			depth++;

			next_to_current = current;
			current = current->next;
		}

		/* No bigger items found, push to end of list */
		m_last = m_last->next = new_node;
	}
	else {
		m_first = m_last = new LinkedList<int>(value);
	}

	return depth;
}

/* O(1) */
optional<PriorityQueueResult> PriorityListQueue::ordered_pop() {
	if(m_first) {
		LinkedList<int>* first = m_first;
		int value = first->item;

		/* Move forward in the queue */
		m_first = first->next;

		/* Delete the item we removed */
		first->next = nullptr;
		delete first;

		/* If we have no first value, we have no last value either */
		if(!m_first) {
			m_last = nullptr;
		}

		return PriorityQueueResult { value, 0 };
	}

	return nullopt;
}