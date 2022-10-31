#pragma once

#include <algorithm>
#include <optional>
#include <stdint.h>

#include "../city.h"

using std::optional, std::nullopt;

static City* cities[54];

optional<int> find_shortest_path(CityPair journey, int max) {
	if(journey.from == journey.to) {
		return 0;
	}

	City** visited_cities = cities;
	while(*visited_cities) {
		if(*visited_cities == journey.from) {
			return nullopt;
		}

		visited_cities++;
	}

	*visited_cities = journey.from;

 	optional<int> shortest = nullopt;

	for(Connection& conn : journey.from->connections) {
		if(max - conn.distance >= 0) {
			optional<int> shortest_from_this_connection = find_shortest_path({conn.city, journey.to}, max - conn.distance);

			if(shortest_from_this_connection.has_value()) {
				shortest = std::min(shortest.value_or(INT_MAX), shortest_from_this_connection.value() + conn.distance);
			}
		}
	}

	*visited_cities = nullptr;
	return shortest;
}

optional<int> find_shortest_path_enhanced(CityPair journey) {
	if(journey.from == journey.to) {
		return 0;
	}

	City** visited_cities = cities;
	while(*visited_cities) {
		if(*visited_cities == journey.from) {
			return nullopt;
		}

		visited_cities++;
	}

	*visited_cities = journey.from;

 	optional<int> shortest = nullopt;

	for(Connection& conn : journey.from->connections) {
		if(conn.distance < shortest.value_or(INT_MAX)) {
			optional<int> shortest_from_this_connection = find_shortest_path_enhanced({conn.city, journey.to});

			if(shortest_from_this_connection.has_value()) {
				shortest = std::min(shortest.value_or(INT_MAX), shortest_from_this_connection.value() + conn.distance);
			}
		}
	}

	*visited_cities = nullptr;
	return shortest;
}