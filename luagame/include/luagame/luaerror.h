#ifndef LUAGAME_LUAERROR_H_
#define LUAGAME_LUAERROR_H_

#include <string>
#include <ostream>

namespace Luagame {
	
	class Lua_error {
		public:
			Lua_error(const std::string& reason, const std::string& message = "")
				: reason(reason), message(message)
			{
			}

			std::string reason;
			std::string message;

			friend std::ostream& operator<<(std::ostream& out, const Lua_error& error);
	};

	std::ostream& operator<<(std::ostream& out, const Lua_error& error);
}

#endif /* LUAGAME_LUAERROR_H_ */
