#pragma once

#include <assert.h>
#include <string.h>

#include <fstream>
#include <vector>

#include "city.h"
#include "hashtable.h"

#define NAME_MOD 541

class Map {
public:
	Map() {
		m_hashtable = new CityHashTable(NAME_MOD);
	}

	~Map() {
		delete m_hashtable;
	}

	bool update_from_file(const char* file) {
		std::ifstream stream(file);

		if(stream.good()) {
			delete m_hashtable;
			m_hashtable = new CityHashTable(NAME_MOD);

			/* Areas to read from file and write to struct  */
			char from[MAX_NAME_LENGTH + 1];
			char to[MAX_NAME_LENGTH + 1];
			char distance_str[16];

			std::string line;

			while(std::getline(stream, line)) {
				memset(from, 0, MAX_NAME_LENGTH + 1);
				memset(to, 0, MAX_NAME_LENGTH + 1);
				memset(distance_str, 0, 4);

				char* writeptr = from;
				int offset = 0;

				for(size_t i = 0; i < line.length(); i++) {
					if(line[i] == ',') {
						if(writeptr == from) {
							writeptr = to;
						}
						else if(writeptr == to) {
							writeptr = distance_str;
						}

						offset = 0;
					}
					else {
						writeptr[offset++] = line[i];
					}
				}

				City* cityA = m_hashtable->get(from);
				if(!cityA) {
					cityA = new City;
					memcpy(cityA->name, from, MAX_NAME_LENGTH + 1);
					m_hashtable->add(cityA);
				}

				City* cityB = m_hashtable->get(to);
				if(!cityB) {
					cityB = new City;
					memcpy(cityB->name, to, MAX_NAME_LENGTH + 1);
					m_hashtable->add(cityB);
				}

				int distance = atoi(distance_str);

				Connection connectionAtoB { cityB, distance };
				Connection connectionBtoA { cityA, distance };

				cityA->connections.push_back(connectionAtoB);
				cityB->connections.push_back(connectionBtoA);
			}

			return true;
		}

		return false;
	};

	CityHashTable& table() {
		return *m_hashtable;
	}

private:
	CityHashTable* m_hashtable;
};