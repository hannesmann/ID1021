/* item.h - Representations of a part of an expression */
#pragma once

#include <string>

enum ItemType {
	ITEM_ADD,
	ITEM_SUB,
	ITEM_MUL,
	ITEM_DIV,
	ITEM_VALUE
};

struct Item {
	ItemType type;
	/* Only relevant if type==ITEM_VALUE */
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

			default:
				return std::to_string(value);
		}
	}
};