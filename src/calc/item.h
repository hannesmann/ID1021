/* item.h - Representations of a part of an expression */
#pragma once

#include <string>
#include <vector>

enum ItemType {
	ITEM_ADD,
	ITEM_SUB,
	ITEM_MUL,
	ITEM_DIV,
	ITEM_VALUE,

	/* For personal number */
	ITEM_MOD10,
	ITEM_MUL_OVERFLOW
};

struct Item {
	ItemType type;
	/* Only relevant if type == ITEM_VALUE */
	int value;

	static Item operation(ItemType type) {
		return Item { type, -1 };
	}

	static Item number(int value) {
		return Item { ITEM_VALUE, value };
	}

	std::string to_text() {
		switch(type) {
			case ITEM_ADD:
				return "+";
			case ITEM_SUB:
				return "-";

			case ITEM_MUL:
				return "*";
			case ITEM_DIV:
				return "/";

			case ITEM_MOD10:
				return "mod10";
			case ITEM_MUL_OVERFLOW:
				return "*'";

			default:
				return std::to_string(value);
		}
	}
};

/* Generates a random math expression for testing the RPN calculator */
static std::vector<Item> generate_test_expression() {
	std::vector<Item> items;

	switch(rand() % 4) {
		/* (x + y) - z */
		case 0:
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::operation(ITEM_ADD));
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::operation(ITEM_SUB));
			break;
		/* (x * y) / z + w */
		case 1:
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::operation(ITEM_MUL));
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::operation(ITEM_DIV));
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::operation(ITEM_ADD));
			break;
		/* x * x * x */
		case 2:
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::operation(ITEM_MUL));
			items.push_back(Item::operation(ITEM_MUL));
			break;
		/* x op y */
		default:
			int op = rand() % ITEM_VALUE;
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::number(rand() % 4 + 2));
			items.push_back(Item::operation((ItemType)op));
			break;
	}

	return items;
}

/* Generates a valid but nonsensical math expression with the specified number of operations for stressing the stack */
static std::vector<Item> generate_benchmark_expression(int operations) {
	std::vector<Item> items;
	items.reserve(operations * 2 + 1);

	for(int i = 0; i < operations + 1; i++) {
		items.push_back(Item::number(rand() % 18 + 3));
	}

	for(int i = 0; i < operations; i++) {
		/* Avoid ITEM_DIV since this can result in divide by zero exception */
		int operation = rand() % (ITEM_VALUE - 1);
		items.push_back(Item::operation((ItemType)operation));
	}

	return items;
}