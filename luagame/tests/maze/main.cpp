#include <luagame.h>
#include <cmath>

#include <lua.hpp>

#include <iostream>
#include <utility>

enum class direction {
	UP, DOWN, LEFT, RIGHT
};

enum class state {
	EMPTY, WALL, START, TARGET
};

struct position {
	int x, y;
};

static position step(position pos, direction dir) {
	if (dir == direction::UP) {
		return {pos.x - 1, pos.y};
	} else if (dir == direction::DOWN) {
		return {pos.x + 1, pos.y};
	} else if (dir == direction::LEFT) {
		return {pos.x, pos.y - 1};
	} else if (dir == direction::RIGHT) {
		return {pos.x, pos.y + 1};
	}
	return { -1, -1 };
}

static void hook(lua_State *, lua_Debug *) {
	std::cout << "Test timed out" << std::endl;
	std::exit(-3);
}

int main(int argc, char *argv[]) try {
	if (argc != 2) {
		return -2;
	}

	Luagame::Luabox box;
	box.open_lib("_G", luaopen_base);

	constexpr auto e = state::EMPTY;
	constexpr auto w = state::WALL;
	constexpr auto s = state::START;
	constexpr auto t = state::TARGET;

	state level[5][5] = {
		{w, w, w, w, w},
		{w, s, e, e, w},
		{w, w, e, w, w},
		{w, e, e, t, w},
		{w, w, w, w, w},
	};

	position cur{1,1};

	box.register_value("UP", int(direction::UP));
	box.register_value("DOWN", int(direction::DOWN));
	box.register_value("LEFT", int(direction::LEFT));
	box.register_value("RIGHT", int(direction::RIGHT));

	box.register_function("go", [&](int dir) {
				auto next = step(cur, direction(dir));
				if (level[next.x][next.y] == w) {
					return false;
				} else if (level[next.x][next.y] == t) {
					std::cout << "Found way!" << std::endl;
					exit(0);
				}
				cur = next;
				return true;
			});

	box.set_hook(hook, Luagame::Luamask::Count, 1000);

	box.run_file(argv[1]);
	return -1;
} catch (Luagame::Lua_error error) {
	std::cerr << error;
	throw;
}
