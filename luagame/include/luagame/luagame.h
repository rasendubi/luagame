#ifndef LUAGAME_LUAGAME_H_
#define LUAGAME_LUAGAME_H_

#include <vector>
#include <boost/coroutine/all.hpp>
#include "luagame/luabox.h"

namespace Luagame {
	/**
	 * This class is just a container for Luaboxes.
	 *
	 * It adds possibilities to run several Luaboxes
	 * simultaneously in the same thread.
	 */
	class Luagame {
		public:
			/**
			 * Moves box into container.
			 */
			void add_luabox(Luabox&& box) {
				boxes.push_back(std::move(box));
			}

			/**
			 * Run all containing Luaboxes.
			 *
			 * Currently it implements simple round-robin strategy.
			 */
			void run();

			/**
			 * Yields execution of current box.
			 * 
			 * Beware that this uses static \c thread_local field,
			 * so that you can't run multiple Luagames in the same thread.
			 */
			static void yield() {
				coro_yield *prev_yield = cur_yield;
				(*cur_yield)();
				cur_yield = prev_yield;
			}

		private:
			std::vector<Luabox> boxes;

			using coro = boost::coroutines::coroutine<void>;
			using coro_call = coro::push_type;
			using coro_yield = coro::pull_type;
			static thread_local coro_yield *cur_yield;
	};
}

#endif /* LUAGAME_LUAGAME_H_ */
