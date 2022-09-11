#include "search.h"
#include <stdlib.h>
#include <stdio.h>

bool linear_search(int key, int* array, int n, bool assume_sorted) {
	for(int i = 0; i < n; i++) {
		if(assume_sorted && array[i] > key) {
			return false;
		}

		if(array[i] == key) {
			return true;
		}
	}

	return false;
}

bool binary_search(int key, int* array, int n) {
	int first = 0;
	int last = n - 1;

	while(true) {
		int i = (first + last) / 2;

		if(array[i] == key) {
			return true;
		}
		else if(array[i] < key && i < last) {
			first = i + 1;
		}
		else if(array[i] > key && i > first) {
			last = i - 1;
		}
		else {
			break;
		}
	}

	return false;
}