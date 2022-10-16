#pragma once

/* From doubly linked list assignment with destructor added */

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <stdio.h>

template<typename T>
struct LinkedList {
	/* Construct list */
	LinkedList(T item) {
		this->item = item;
		this->next = nullptr;
	}

	~LinkedList() {
		/* Recursive */
		if(next) {
			delete next;
		}
	}

	T item;
	LinkedList<T>* next;

	/* Iterates through list to find last element */
	LinkedList<T>* last() {
		LinkedList<T>* current = this;

		while(current && current->next) {
			current = current->next;
		}

		return current;
	}

	/* Append element to end of list */
	void append_to_end(LinkedList<T>* list) {
		last()->next = list;
	}

	/*
	 * Remove item, should only be called on the first element in the list
	 * Returns first element in list (may otherwise be lost if item == first)
	 */
	LinkedList<T>* remove(LinkedList<T>* item) {
		LinkedList<T>* first = this;

		/* If we are the first item, disconnect from list */
		if(this == item) {
			first = next;
			next = nullptr;
		}
		else {
			LinkedList<T>* next_to_item = this;

			while(next_to_item->next) {
				/* We found the item before the one we're looking for */
				if(next_to_item->next == item) {
					/* Move item->next back one position in the list */
					next_to_item->next = item->next;
					break;
				}

				next_to_item = next_to_item->next;
			}
		}

		return first;
	}
};

template<typename T>
struct DoublyLinkedList {
	/* Construct list */
	DoublyLinkedList(T item) {
		this->item = item;
		this->previous = nullptr;
		this->next = nullptr;
	}

	~DoublyLinkedList() {
		/* Recursive */
		if(next) {
			delete next;
		}
	}

	T item;
	DoublyLinkedList<T> *previous, *next;

	/* Iterates through list to find first element */
	DoublyLinkedList<T>* first() {
		DoublyLinkedList<T>* current = this;

		while(current->previous) {
			current = current->previous;
		}

		return current;
	}

	/* Iterates through list to find last element */
	DoublyLinkedList<T>* last() {
		DoublyLinkedList<T>* current = this;

		while(current->next) {
			current = current->next;
		}

		return current;
	}

	/* Append element to end of list */
	void append_to_end(DoublyLinkedList<T>* list) {
		DoublyLinkedList<T>* last_element = last();
		last_element->next = list;
		list->previous = last_element;
	}

	/* Remove this item from the list */
	void remove_self() {
		if(previous) {
			previous->next = next;
		}
		if(next) {
			next->previous = previous;
		}
	}
};
