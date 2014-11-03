#include "luagame/function_wrapper_impl.h"

#include <type_traits>

namespace Luagame_impl {
	template<>
	lua_State *get_arg(lua_State *state, int) {
		return state;
	}

	template<>
	int get_arg(lua_State *state, int pos) {
		return lua_tonumber(state, pos);
	}

	template<>
	double get_arg(lua_State *state, int pos) {
		return lua_tonumber(state, pos);
	}

	template<>
	const char *get_arg(lua_State *state, int pos) {
		return lua_tostring(state, pos);
	}

	template<>
	std::string get_arg(lua_State *state, int pos) {
		return get_arg<const char *>(state, pos);
	}

	template<>
	int put_result(lua_State *state, int res) {
		lua_pushnumber(state, res);
		return 1;
	}

	template<>
	int put_result(lua_State * state, double res) {
		lua_pushnumber(state, res);
		return 1;
	}

}
