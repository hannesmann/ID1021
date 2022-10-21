#pragma once

#include <algorithm>
#include <vector>
#include <fstream>
#include <string.h>

#define MAX_ZIP_LENGTH 6
/* Needs to be longer because of UTF-8 */
#define MAX_AREA_LENGTH 40
#define MAX_POP_LENGTH 10

#define MAX_ZIP 99999

#define MAX_COLLISIONS 16

/* Defines properties for a particular Swedish zip code */
struct ZipCode {
	/* The code itself */
	char codestr[MAX_ZIP_LENGTH + 1];
	int codeint;
	/* The name of the area where the zip code is located */
	char area[MAX_AREA_LENGTH + 1];
	/* The number of people that can receive post with this zip code */
	int population;
};

/* Reads and stores a list of zip codes */
class ZipCodeTable {
public:
	ZipCodeTable() {
		m_lookup_table = new ZipCode*[MAX_ZIP];
		memset(m_lookup_table, 0, MAX_ZIP * sizeof(ZipCode*));
	}

	~ZipCodeTable() {
		delete[] m_lookup_table;
	}

	bool update_from_file(const char* name) {
		std::ifstream stream(name);

		if(stream.good()) {
			m_table.clear();
			memset(m_lookup_table, 0, MAX_ZIP * sizeof(ZipCode*));

			/* Areas to read from file and write to struct  */
			char code[MAX_ZIP_LENGTH + 1];
			char area[MAX_AREA_LENGTH + 1];
			char population[MAX_POP_LENGTH + 1];

			std::string line;

			while(std::getline(stream, line)) {
				memset(code, 0, MAX_ZIP_LENGTH + 1);
				memset(area, 0, MAX_AREA_LENGTH + 1);
				memset(population, 0, MAX_POP_LENGTH + 1);

				char* writeptr = code;
				int offset = 0;

				for(size_t i = 0; i < line.length(); i++) {
					if(line[i] == ',') {
						if(writeptr == code) {
							writeptr = area;
						}
						else if(writeptr == area) {
							writeptr = population;
						}

						offset = 0;
					}
					else {
						writeptr[offset++] = line[i];
					}
				}

				ZipCode zip;
				memcpy(zip.codestr, code, MAX_ZIP_LENGTH + 1);
				/* Not the best solution... but we know zip codes will always be formatted like this */
				zip.codeint = atoi(&zip.codestr[0]) * 100 + atoi(&zip.codestr[4]);
				memcpy(zip.area, area, MAX_AREA_LENGTH + 1);
				zip.population = atoi(population);

				m_table.push_back(zip);
			}

			/* We have to update the lookup table at the end because m_table.push_back could reallocate */
			for(ZipCode& zip : m_table) {
				m_lookup_table[zip.codeint] = &zip;
			}

			return true;
		}

		return false;
	};

	ZipCode* lookup_linear(const char* key) {
		for(ZipCode& zip : m_table) {
			if(!strcmp(zip.codestr, key)) {
				return &zip;
			}
		}

		return nullptr;
	}

	ZipCode* lookup_binary(const char* key) {
		int start = 0;
		int end = m_table.size() - 1;

		while(start != end) {
			int middle = start + (end - start) / 2;
			int result = strcmp(key, m_table[middle].codestr);

			/* Key greater than middle */
			if(result > 0) {
				start = middle + 1;
			}
			/* Key less than middle */
			else if(result < 0) {
				end = middle - 1;
			}
			else {
				return &m_table[middle];
			}
		}

		return nullptr;
	}

	ZipCode* lookup_linear(int key) {
		for(ZipCode& zip : m_table) {
			if(zip.codeint == key) {
				return &zip;
			}
		}

		return nullptr;
	}

	ZipCode* lookup_binary(int key) {
		int start = 0;
		int end = m_table.size() - 1;

		while(start != end) {
			int middle = start + (end - start) / 2;

			if(key > m_table[middle].codeint) {
				start = middle + 1;
			}
			else if(key < m_table[middle].codeint) {
				end = middle - 1;
			}
			else {
				return &m_table[middle];
			}
		}

		return nullptr;
	}

	ZipCode* lookup_fast(int key) {
		return m_lookup_table[key];
	}

	/* 0..99999 (MAX_ZIP) */
	ZipCode** zips() {
		return m_lookup_table;
	}

	/* Counts type of collision (1 key maps to same idx, 2 keys map to same idx, etc...)*/
	int* collisions(int mod) {
		int* data = new int[mod];
		int* collisions = new int[MAX_COLLISIONS];

		memset(data, 0, mod * sizeof(int));
		memset(collisions, 0, MAX_COLLISIONS * sizeof(int));

		for(const ZipCode& zip : m_table) {
			int idx = zip.codeint % mod;

			/* If we have too many collisions on the same index, quit */
			assert(data[idx] < MAX_COLLISIONS);
			collisions[data[idx]]++;

			data[idx]++;
		}

		delete[] data;
		return collisions;
	}

	int find_best_mod(int rangestart, int rangeend) {
		/*
		 * The algorithm works like this:
		 * 1. Does X have a lower max collision type than Y? If yes, it's better.
		 * 2. Otherwise, does X have a lower "cost" than Y? Cost is calculated by n * collision_type[n] * 1.5
		 */
		int best = -1;
		float best_cost = -1;
		int best_max_collision_type = -1;

		for(int i = rangestart; i <= rangeend; i++) {
			int* ctypes = collisions(i);

			float cost = 0;
			int max_collision_type = 0;

			for(int j = 0; j < MAX_COLLISIONS; j++) {
				if(ctypes[j]) {
					cost += ctypes[j] * 1.5f;
					max_collision_type++;
				}
				else {
					break;
				}
			}

			delete[] ctypes;

			/* If we found a hash with no collisions, it can't get any better */
			if(max_collision_type == 0) {
				best = i;
				break;
			}

			if(best == -1 || max_collision_type < best_max_collision_type || cost < best_cost) {
				best = i;
				best_cost = cost;
				best_max_collision_type = max_collision_type;
			}
		}

		return best;
	}
private:
	std::vector<ZipCode> m_table;
	ZipCode** m_lookup_table;
};