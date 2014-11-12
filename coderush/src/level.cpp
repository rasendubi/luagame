#include "level.h"

#include <set>

#include "luagame/luabox.h"

Coderush::Level Coderush::load_level(const std::string& filename) {
	Luagame::Luabox box;
	box.run_file(filename);
	std::string map_str = box.get<std::string>("map");

	Level::Map map;
	Level::position start;
	Level::position end;

	std::vector<Level::State> cur_row;
	int x = 0;
	int y = 0;
	for (auto c : map_str) {
		if (std::set<char>{'\n', 'x', 's', 't', ' '}.count(c) == 0) {
			// TODO: throw exception?
			continue;
		}

		if (c == '\n') {
			map.push_back(cur_row);
			cur_row.clear();
			y = 0;
			++x;
		} else if (c == 'x') {
			cur_row.push_back(Level::State::WALL);
			++y;
		} else {
			if (c == 's') {
				start = {x, y};
			} else if (c == 't') {
				end = {x, y};
			}
			cur_row.push_back(Level::State::EMPTY);
			++y;
		}
	}
	if (!cur_row.empty()) {
		map.push_back(cur_row);
	}

	return { map, start, end };
}
