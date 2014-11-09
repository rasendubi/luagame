#include <gtest/gtest.h>
#include <boost/coroutine/all.hpp>
#include "luagame/luabox.h"

using coro = boost::coroutines::coroutine<void>;

using coro_yield = coro::pull_type;
using coro_call = coro::push_type;

thread_local coro_yield *yield;

void hook(lua_State *state, lua_Debug *ar) {
	(void)state;
	(void)ar;
	(*yield)();
}

TEST(boost, coroutine) {
	Luagame::Luabox box;
	box.set_hook(hook, Luagame::Luamask::Count, 1000);

	coro_call run(
			[&](coro_yield& yield) {
				::yield = &yield;
				box.run_file("files/loop.lua");
			});

	int count = 0;
	for (int i = 0; i < 5; ++i) {
		run();
		++count;
	}

	EXPECT_EQ(5, count);
}
