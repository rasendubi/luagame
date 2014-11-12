#include "level.h"
#include <gtest/gtest.h>
#include <utility>

TEST(Level, load) {
	Coderush::Level level = Coderush::load_level("files/level.lua");

	constexpr auto e = Coderush::Level::State::EMPTY;
	constexpr auto w = Coderush::Level::State::WALL;

	Coderush::Level::Map expected = {
		{ w,w,w,w,w,w,w, },
		{ w,e,w,e,w,e,w, },
		{ w,e,e,e,w,e,w, },
		{ w,e,w,e,e,e,w, },
		{ w,w,w,w,w,w,w, },
	};

	for (size_t i = 0; i < expected.size(); ++i) {
		for (size_t j = 0; j < expected[i].size(); ++j) {
			EXPECT_EQ(expected[i][j], level[i][j]);
		}
	}

	Coderush::Level::position expected_start = {3,1};
	Coderush::Level::position expected_end = {1,5};

	EXPECT_EQ(expected_start, level.current_position());
	EXPECT_EQ(expected_end, level.exit_position());
}
