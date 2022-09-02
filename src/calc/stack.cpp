#include "stack.h"
#include <assert.h>
#include <stdio.h>

FixedStack::FixedStack(int n) {
	m_stack = (int*) malloc(n * sizeof(int));
	m_available_slots = n;
}

void FixedStack::push(int value) {
#ifdef DEBUG
	printf("FixedStack(%d)::push() - m_top_of_stack=%d\n", m_available_slots, m_top_of_stack);
#endif

	assert(m_top_of_stack < m_available_slots - 1);
	m_stack[++m_top_of_stack] = value;
}

int FixedStack::pop() {
#ifdef DEBUG
	printf("FixedStack(%d)::pop() - m_top_of_stack=%d\n", m_available_slots, m_top_of_stack);
#endif

	assert(m_top_of_stack >= 0);
	return m_stack[m_top_of_stack--];
}

DynamicStack::DynamicStack() {
	m_stack = (int*) malloc(4 * sizeof(int));
	m_available_slots = 4;
}

void DynamicStack::push(int value) {
#ifdef DEBUG
	printf("DynamicStack(%d)::push() - m_top_of_stack=%d\n", m_available_slots, m_top_of_stack);
#endif

	/* Grow stack BEFORE adding items */
	if(m_top_of_stack >= m_available_slots - 1) {
		/* 4, 8, 16, 32, 64, 128, 256, 512, 1024, etc */
		m_available_slots *= 2;
		m_stack = (int*) realloc(m_stack, m_available_slots * sizeof(int));
#ifdef DEBUG
		printf("DynamicStack::push() - Grew stack to %d\n", m_available_slots);
#endif
	}

	m_stack[++m_top_of_stack] = value;
}

int DynamicStack::pop() {
#ifdef DEBUG
	printf("DynamicStack(%d)::pop() - m_top_of_stack=%d\n", m_available_slots, m_top_of_stack);
#endif

	assert(m_top_of_stack >= 0);
	int value = m_stack[m_top_of_stack--];

	/* Shrink stack AFTER removing items */
	if(m_top_of_stack < m_available_slots / 2 && m_available_slots > 4) {
		/* Reverse logic of push() */
		m_available_slots /= 2;
		m_stack = (int*) realloc(m_stack, m_available_slots * sizeof(int));
#ifdef DEBUG
		printf("DynamicStack::pop() - Shrank stack to %d\n", m_available_slots);
#endif
	}

	return value;
}