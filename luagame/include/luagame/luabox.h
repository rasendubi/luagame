#ifndef LUAGAME_LUABOX_H_
#define LUAGAME_LUABOX_H_

#include <memory>

struct lua_State;

namespace Luagame {


	/**
	 * This is a base class for all Lua robots.
	 */
	class Luabox {
		public:
			/**
			 * \throw Lua_error unable to allocate lua state
			 */
			Luabox();

			Luabox(const Luabox&) = delete;
			Luabox(Luabox&& box);

			Luabox& operator=(const Luabox&) = delete;
			Luabox& operator=(Luabox&& box);

			void run_file(const char *filename);

		private:
			std::unique_ptr<lua_State, void (*)(lua_State *)> state;
	};
}

#endif /* LUAGAME_LUABOX_H_ */

