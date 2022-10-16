#pragma once

#include <assert.h>
#include <algorithm>
#include <stdio.h>

#include "list.h"

/* Divides array into two separate partitions, one with smaller or equal elements to ptr[pivot] and the other with larger elements */
int partition_array(int* ptr, int n) {
	int pivot = *ptr;

	int before_pivot = 0;
	int after_or_equal_pivot = 0;

	while(true) {
		for(before_pivot = 0; before_pivot < n; before_pivot++) {
			if(ptr[before_pivot] > pivot) {
				break;
			}
		}

		for(after_or_equal_pivot = n - 1; after_or_equal_pivot >= 0; after_or_equal_pivot--) {
			if(ptr[after_or_equal_pivot] <= pivot) {
				break;
			}
		}

		if(before_pivot <= after_or_equal_pivot) {
			std::swap(ptr[before_pivot], ptr[after_or_equal_pivot]);
		}
		else {
			break;
		}
	}

	/* ptr[pivot] is now guaranteed to be equal to the pivot */
	std::swap(ptr[0], ptr[after_or_equal_pivot]);
	return after_or_equal_pivot;
}

/* Sorts all elements from ptr[0] to ptr[n-1] */
void quicksort_array(int* ptr, int n) {
	if(n < 2) {
		/* We're done */
		return;
	}

	int pivot = partition_array(ptr, n);

	quicksort_array(ptr, pivot);
	quicksort_array((ptr + pivot + 1), n - pivot - 1);
}

/* Provides access to both head and tail of list */
template<typename T>
struct LinkedListReference {
	LinkedList<T> *begin, *end;
};

/* Sorts a linked list (begin -> end) */
LinkedListReference<int> quicksort_linked_list(LinkedList<int>* list) {
	if(!list || !list->next) {
		/* We're done */
		return { list, list };
	}

	/* Store references to both start and end for faster appending */
	LinkedListReference<int> before_pivot = { nullptr, nullptr };
	LinkedListReference<int> after_or_equal_pivot = { nullptr, nullptr };

	LinkedList<int>* next = list->next;

	while(next) {
		/* We need to edit next->next, so save this here */
		LinkedList<int>* node_to_advance_to = next->next;
		/* Push into two separate lists depending on condition */
		bool smaller_than_pivot = next->item < list->item;
		LinkedListReference<int>* list_to_push_into = smaller_than_pivot ? &before_pivot : &after_or_equal_pivot;

		next->next = list_to_push_into->begin;
		list_to_push_into->begin = next;

		/* Elements are pushed into the list backwards, so the beginning will eventually become the end */
		if(!list_to_push_into->end) {
			list_to_push_into->end = list_to_push_into->begin;
		}

		next = node_to_advance_to;
	}

	before_pivot = quicksort_linked_list(before_pivot.begin);
	after_or_equal_pivot = quicksort_linked_list(after_or_equal_pivot.begin);

	/* Merge at end */
	if(before_pivot.end) {
		before_pivot.end->next = list;
	}
	list->next = after_or_equal_pivot.begin;

	return {
		before_pivot.begin ? before_pivot.begin : list,
		after_or_equal_pivot.begin ? after_or_equal_pivot.end : list
	};
}