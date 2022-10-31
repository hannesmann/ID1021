#pragma once

#include <algorithm>
#include <optional>
#include <stdint.h>

#include "../city.h"

using std::optional, std::nullopt;

optional<int> find_shortest_path_naive(CityPair journey, int max) {
	if(journey.from == journey.to) {
		return 0;
	}

	optional<int> shortest = nullopt;

	for(Connection& conn : journey.from->connections) {
		if(max - conn.distance >= 0) {
			optional<int> shortest_from_this_connection = find_shortest_path_naive({conn.city, journey.to}, max - conn.distance);

			if(shortest_from_this_connection.has_value()) {
				shortest = std::min(shortest.value_or(INT_MAX), shortest_from_this_connection.value() + conn.distance);
			}
		}
	}

	return shortest;
}