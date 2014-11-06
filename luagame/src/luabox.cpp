#include "luagame/luabox.h"
#include "luagame/luaerror.h"

#include <lua.hpp>

Luagame::Luabox::Luabox() throw (Luagame::Lua_error)
		: state{luaL_newstate(), lua_close} {
	if (!state) {
		throw Lua_error("Unable to allocate lua state");
	}
}

Luagame::Luabox::Luabox(Luabox&& box)
		: state(std::move(box.state)) {
}

Luagame::Luabox& Luagame::Luabox::operator=(Luagame::Luabox&& box) {
	state = std::move(box.state);
	return *this;
}

void Luagame::Luabox::run_file(const char *filename) {
	int ret = luaL_dofile(state.get(), filename);
	if (ret) {
		throw Lua_error(std::string("Error executing file ") + filename, lua_tostring(state.get(), -1));
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

void Luagame::Luabox::set_hook(lua_Hook func, Luagame::Luamask mask, int count) {
	lua_sethook(state.get(), func, static_cast<int>(mask), count);
}
