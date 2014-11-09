#include "luagame/luagame.h"

thread_local Luagame::Luagame::coro_yield *Luagame::Luagame::cur_yield = nullptr;

void Luagame::Luagame::run() {
	std::vector<coro_call> coroutines;
	for (auto& box : boxes) {
		coroutines.push_back(coro_call(
					[&](coro_yield& yield) {
						cur_yield = &yield;
						box.run();
					}));
	}

	bool one_run;
	do {
		one_run = false;
		for (auto& coro : coroutines) {
			if (coro) {
				one_run = true;
				coro();
			}
		}
	} while (one_run);
}
