#include "luagame/luabox.h"
#include "luagame/luaerror.h"

#include <lua.hpp>

Luagame::Luabox::Luabox()
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
