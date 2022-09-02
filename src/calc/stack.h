/* stack.h - Implementation of static and dynamic stack */
#pragma once

#include <stdlib.h>

class Stack {
public:
	virtual ~Stack() {}

 	virtual int pop() = 0;
	virtual void push(int value) = 0;

	virtual const char* describe() = 0;
};

/* Fixed size stack containing n slots */
class FixedStack : public Stack {
public:
	FixedStack(int n);
	~FixedStack() override { free(m_stack); }

 	int pop() override;
	void push(int value) override;

	const char* describe() override { return "fixed stack"; }
private:
	int* m_stack;

	/* This points at the last element in the stack */
	int m_top_of_stack = -1;
	int m_available_slots = 0;
};

/* Dynamic stack that grows and shrinks as item are pushed and popped  */
class DynamicStack : public Stack {
public:
	DynamicStack();
	~DynamicStack() override { free(m_stack); }

 	int pop() override;
	void push(int value) override;

	const char* describe() override { return "dynamic stack"; }
private:
	int* m_stack;

	/* This points at the last element in the stack */
	int m_top_of_stack = -1;
	int m_available_slots = 0;
};