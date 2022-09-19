#include "algorithms.h"
#include <algorithm>

void selection_sort(int* array, int n) {
	for(int i = 0; i < n - 1; i++) {
		int min = i;

		for(int j = i; j < n; j++) {
			if(array[j] < array[min]) {
				min = j;
			}
		}

		std::swap(array[i], array[min]);
	}
}

void insertion_sort(int* array, int n) {
	for(int i = 0; i < n; i++) {
		for(int j = i; j > 0 && array[j] < array[j - 1]; j--) {
			std::swap(array[j], array[j - 1]);
		}
	}
}

void merge_sort_merge(int* orig, int* copy, int lo, int mid, int hi) {
	std::copy(orig+lo, orig+hi+1, copy+lo);

	int i = lo;
	int j = mid+1;

	for(int k = lo; k <= hi; k++) {
		if(i > mid) {
			orig[k] = copy[j++];
		}
		else if(j > hi) {
			orig[k] = copy[i++];
		}
		else if(copy[i] < copy[j]) {
			orig[k] = copy[i++];
		}
		else {
			orig[k] = copy[j++];
		}
	}
}

void merge_sort_private(int* orig, int* copy, int lo, int hi) {
	if(lo != hi) {
		int mid = lo + (hi-lo) / 2;

		merge_sort_private(orig, copy, lo, mid);
		merge_sort_private(orig, copy, mid+1, hi);
		merge_sort_merge(orig, copy, lo, mid, hi);
	}
}

void merge_sort(int* array, int n) {
	if(n > 0) {
		int* copy = new int[n];
		merge_sort_private(array, copy, 0, n - 1);
		delete[] copy;
	}
}