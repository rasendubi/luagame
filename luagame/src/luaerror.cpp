#include "luagame/luaerror.h"

std::ostream& Luagame::operator<<(std::ostream& out, const Lua_error& error) {
	return out << "Lua Error: " << error.reason << std::endl << error.message << std::endl;
}
