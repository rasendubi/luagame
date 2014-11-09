#include "luagame/luabox.h"
#include "luagame/luaerror.h"

#include <lua.hpp>

Luagame::Luabox::Luabox() throw (Luagame::Lua_error)
		: state{luaL_newstate(), lua_close} {
	if (!state) {
		throw Lua_error("Unable to allocate lua state");
	}
}

void Luagame::Luabox::run_file(const std::string& filename) {
	int ret = luaL_dofile(state.get(), filename.c_str());
	if (ret) {
		throw Lua_error("Error executing file " + filename,
				lua_tostring(state.get(), -1));
	}
}

void Luagame::Luabox::set_file(const std::string& filename) {
	int ret = luaL_loadfile(state.get(), filename.c_str());
	if (ret) {
		throw Lua_error("Error loading file " + filename,
				lua_tostring(state.get(), -1));
	}
}

void Luagame::Luabox::run() {
	int ret = lua_pcall(state.get(), 0, LUA_MULTRET, 0);
	if (ret) {
		throw Lua_error("Error running",
				lua_tostring(state.get(), -1));
	}
}

void Luagame::Luabox::open_lib(const char *name, openlib_func lib) {
	luaL_requiref(state.get(), name, lib, 1);
	lua_pop(state.get(), 1);
}

void Luagame::Luabox::register_function(const std::string& name, lua_CFunction function) {
	lua_pushcfunction(state.get(), function);
	lua_setglobal(state.get(), name.c_str());
}
