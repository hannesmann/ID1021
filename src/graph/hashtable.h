#pragma once

/* Adapted from hash table assignment */

#include <assert.h>
#include <algorithm>
#include <stdint.h>
#include <string.h>

#include "city.h"

struct CityHashTableBucket {
	/* Number of elements in bucket */
	uint8_t n = 0;
	/* Cities array */
	City** cities = nullptr;

	~CityHashTableBucket() {
		if(cities) {
			free(cities);
		}
	}

	City* find(const char* key) {
		/* Always do linear search to make sure the key actually exists */
		for(uint8_t i = 0; i < n; i++) {
			if(!strcmp(cities[i]->name, key)) {
				return cities[i];
			}
		}

		return nullptr;
	}

	void add(City* city) {
		assert(n < UINT8_MAX);
		n++;

		if(cities) {
			cities = (City**)realloc(cities, sizeof(City*) * n);
		}
		else {
			cities = (City**)malloc(sizeof(City*) * n);
		}

		cities[n - 1] = city;
	}

	void remove(const char* key) {
		if(n == 1) {
			free(cities);
			cities = nullptr;
		}
		else {
			bool swap = false;

			for(uint8_t i = 0; i < n; i++) {
				if(!strcmp(cities[i]->name, key)) {
					swap = true;
				}
				else if(swap) {
					std::swap(cities[i - 1], cities[i]);
				}
			}

			cities = (City**)realloc(cities, sizeof(City*) * (n - 1));
		}

		n--;
	}
};

class CityHashTable {
public:
	CityHashTable(int mod) : m_mod(mod) {
		m_buckets = new CityHashTableBucket[mod];
	}

	~CityHashTable() {
		delete[] m_buckets;
	}

	void add(City* city) {
		assert(city);
		assert(!has(city->name));

		m_buckets[hash(city->name)].add(city);
	}

	void remove(const char* city) {
		assert(has(city));

		m_buckets[hash(city)].remove(city);
	}

	City* get(const char* city) {
		return m_buckets[hash(city)].find(city);
	}

	inline bool has(const char* city) {
		return !!get(city);
	}

	vector<City*> all() {
		vector<City*> result;

		for(int i = 0; i < m_mod; i++) {
			for(int j = 0; j < m_buckets[i].n; j++) {
				result.push_back(m_buckets[i].cities[j]);
			}
		}

		return result;
	}
private:
	inline int hash(const char* name) {
		int result = 7;

		while(*name != '\0') {
			result = (result*31 % m_mod) + *name;
			name++;
		}

		return result % m_mod;
	}

	CityHashTableBucket* m_buckets;
	int m_mod;
};