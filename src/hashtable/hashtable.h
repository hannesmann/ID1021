#pragma once

#include <assert.h>
#include <algorithm>
#include "ziptable.h"

struct ZipHashTableBucket {
	/* Number of elements in bucket */
	uint8_t n = 0;
	/* Codes array */
	ZipCode** codes = nullptr;

	~ZipHashTableBucket() {
		if(codes) {
			free(codes);
		}
	}

	ZipCode* find(int key) {
		/* Always do linear search to make sure the key actually exists */
		for(uint8_t i = 0; i < n; i++) {
			if(codes[i]->codeint == key) {
				return codes[i];
			}
		}

		return nullptr;
	}

	void add(ZipCode* zip) {
		assert(n < UINT8_MAX);
		n++;

		if(codes) {
			codes = (ZipCode**)realloc(codes, sizeof(ZipCode*) * n);
		}
		else {
			codes = (ZipCode**)malloc(sizeof(ZipCode*) * n);
		}

		codes[n - 1] = zip;
	}

	void remove(int key) {
		if(n == 1) {
			free(codes);
			codes = nullptr;
		}
		else {
			bool swap = false;

			for(uint8_t i = 0; i < n; i++) {
				if(codes[i]->codeint == key) {
					swap = true;
				}
				else if(swap) {
					std::swap(codes[i - 1], codes[i]);
				}
			}

			codes = (ZipCode**)realloc(codes, sizeof(ZipCode*) * (n - 1));
		}

		n--;
	}
};

class ZipHashTableBase {
public:
	virtual ~ZipHashTableBase() { }

	virtual void add(ZipCode* zip) = 0;
	virtual void remove(int zip) = 0;

	virtual ZipCode* get(int zip) = 0;

	inline bool has(int zip) {
		return !!get(zip);
	}
};

class ZipHashTable : public ZipHashTableBase {
public:
	ZipHashTable(int mod) : m_mod(mod) {
		m_buckets = new ZipHashTableBucket[mod];
	}

	~ZipHashTable() override {
		delete[] m_buckets;
	}

	void add(ZipCode* zip) override {
		assert(zip);
		assert(!has(zip->codeint));

		m_buckets[hash(zip->codeint)].add(zip);
	}

	void remove(int zip) override {
		assert(has(zip));

		m_buckets[hash(zip)].remove(zip);
	}

	ZipCode* get(int zip) override {
		return m_buckets[hash(zip)].find(zip);
	}
private:
	inline int hash(int key) {
		return key % m_mod;
	}

	ZipHashTableBucket* m_buckets;
	int m_mod;
};

class ZipArrayHashTable : public ZipHashTableBase {
public:
	ZipArrayHashTable(int mod, int expected_duplicates) :
		m_mod(mod),
		/* Add 1 to expected_duplicates so expected_duplicates==0 => m_slots=m_mod */
		m_expected_duplicates(expected_duplicates + 1) {
		m_slots = new ZipCode*[available_slots()];
		memset(m_slots, 0, available_slots() * sizeof(ZipCode*));
	}

	int last_depth = 0;

	virtual ~ZipArrayHashTable() {
		delete[] m_slots;
	}

	void add(ZipCode* zip) {
		assert(zip);

		int start = hash(zip->codeint) * m_expected_duplicates;
		int current = start * m_expected_duplicates;

		while(m_slots[current % available_slots()]) {
			assert(m_slots[current % available_slots()] != zip);

			current++;

			/* Wrapped around, array has been filled */
			assert(current % available_slots() != start);
		}

		m_slots[current % available_slots()] = zip;
	}

	void remove(int zip) {
		int start = hash(zip) * m_expected_duplicates;
		int current = start * m_expected_duplicates;

		while(m_slots[current % available_slots()]) {
			if(m_slots[current % available_slots()]->codeint == zip) {
				m_slots[current % available_slots()] = nullptr;
				break;
			}

			current++;

			/* Wrapped around, no match */
			if(current % available_slots() == start) {
				break;
			}
		}
	}

	ZipCode* get(int zip) {
		int start = hash(zip) * m_expected_duplicates;
		int current = start * m_expected_duplicates;

		last_depth = 0;

		while(m_slots[current % available_slots()]) {
			if(m_slots[current % available_slots()]->codeint == zip) {
				return m_slots[current % available_slots()];
			}

			last_depth++;
			current++;

			/* Wrapped around, no match */
			if(current % available_slots() == start) {
				break;
			}
		}

		return nullptr;
	}
private:
	inline int hash(int key) {
		return key % m_mod;
	}

	inline int available_slots() {
		return m_mod * m_expected_duplicates;
	}

	ZipCode** m_slots;

	int m_mod;
	int m_expected_duplicates;
};