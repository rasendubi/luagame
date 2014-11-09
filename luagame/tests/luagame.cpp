#include <gtest/gtest.h>
#include "luagame/luagame.h"

using Luagame::Luagame;
using Luagame::Luabox;

TEST(luagame, run) {
	class Luagame game;
	Luabox box1{"files/box1.lua"};
	Luabox box2{"files/box2.lua"};

	int val = -1;
	box1.register_function("give", [&](int value) {
				val = value;
				Luagame::Luagame::yield();
			});

	box2.register_function("get", [&]() {
				while (val == -1) {
					Luagame::Luagame::yield();
				}
				int result = val;
				Luagame::Luagame::yield();
				return result;
			});

	std::vector<int> answers;
	box2.register_function("notify", [&](int answer) {
				answers.push_back(answer);
			});

	game.add_luabox(std::move(box1));
	game.add_luabox(std::move(box2));

	game.run();

	ASSERT_EQ(2, answers.size());
	EXPECT_EQ(13, answers[0]);
	EXPECT_EQ(42, answers[1]);
}
