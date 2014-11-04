#include "luagame/luabox.h"
#include <gtest/gtest.h>

using Luagame::Luabox;

TEST(luabox, constructors) {
	Luabox box1;
	{
		Luabox box2;
		box1 = std::move(box2);
	}
}

struct mult_with {
	int with;

	int operator()(int a) {
		return a * with;
	}
};

double l_sum(double a, double b) {
	return a + b;
}

TEST(luabox, register_function) {
	Luabox box;

	std::string result;
	box.register_function("double", mult_with{2});
	box.register_function("sum", l_sum);
	box.register_function("print", [&](std::string str) {
				result = str;
			});

	box.run_file("tests/files/register_function.lua");

	EXPECT_EQ("21", result);
}

TEST(luabox, register_value) {
	Luabox box;

	box.register_value("A", 15);
	box.register_value("BB", "hello");

	int A;
	std::string BB;
	box.register_function("a_is", [&](int a) { A = a; });
	box.register_function("bb_is", [&](std::string bb) { BB = bb; });

	box.run_file("tests/files/register_value.lua");

	EXPECT_EQ(15, A);
	EXPECT_EQ("hello", BB);
}

TEST(luabox, open_lib) {
	Luabox box;

	box.open_lib("math", luaopen_math);

	int result;
	box.register_function("finish", [&](int a){ result = a; });

	box.run_file("tests/files/open_lib.lua");

	EXPECT_EQ(1, result);
}
