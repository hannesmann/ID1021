#pragma once

#include <vector>

#define MAX_NAME_LENGTH 64

using std::vector;

class City;

struct Connection {
	City* city;
	int distance;
};

struct City {
	char name[MAX_NAME_LENGTH + 1];
	vector<Connection> connections;
};

struct CityPair {
	City *from, *to;
};
