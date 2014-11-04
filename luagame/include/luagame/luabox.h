#ifndef LUAGAME_LUABOX_H_
#define LUAGAME_LUABOX_H_

#include <memory>
#include <type_traits>

extern "C" {
	#include <lualib.h>
}

#include "luagame/luaerror.h"
#include "luagame/function_wrapper_impl.h"

namespace Luagame {


	/**
	 * This is a base class for all Lua robots.
	 */
	class Luabox {
		public:
			using openlib_func = int (*)(lua_State*);

			/**
			 * \throw Lua_error unable to allocate lua state
			 */
			Luabox() throw (Lua_error);

			Luabox(const Luabox&) = delete;
			Luabox(Luabox&& box);

			Luabox& operator=(const Luabox&) = delete;
			Luabox& operator=(Luabox&& box);

			void run_file(const char *filename);

			void open_lib(const char *name, openlib_func lib);

			void register_function(const std::string& name, lua_CFunction function);

			template<typename F>
			typename std::enable_if<!std::is_pod<F>::value, void>::type
					register_function(const std::string& name, F function) {
				using Luagame_impl::function_wrapper;
				void *data = lua_newuserdata(state.get(), sizeof(F));
				new(data) F(std::forward<F>(function));

				lua_createtable(state.get(), 0, 1);
				lua_pushcfunction(state.get(), function_wrapper<F>::destroyer);
				lua_setfield(state.get(), -2, "__gc");
				lua_setmetatable(state.get(), -2);

				lua_pushcclosure(state.get(), function_wrapper<F>::caller, 1);
				lua_setglobal(state.get(), name.c_str());
			}

			template<typename F>
			typename std::enable_if<std::is_pod<F>::value && !std::is_pointer<F>::value, void>::type
					register_function(const std::string& name, F function) {
				using Luagame_impl::function_wrapper;
				void *data = lua_newuserdata(state.get(), sizeof(F));
				new(data) F(function);

				lua_pushcclosure(state.get(), function_wrapper<F>::caller, 1);
				lua_setglobal(state.get(), name.c_str());
			}

			template<typename F>
			typename std::enable_if<std::is_pod<F>::value && std::is_pointer<F>::value, void>::type
					register_function(const std::string& name, F function) {
				using Luagame_impl::function_wrapper;
				lua_pushlightuserdata(state.get(), reinterpret_cast<void *>(function));
				lua_pushcclosure(state.get(), function_wrapper<F>::caller, 1);
				lua_setglobal(state.get(), name.c_str());
			}

			template<typename T>
			void register_value(const std::string& name, T value) {
				Luagame_impl::put_value(state.get(), value);
				lua_setglobal(state.get(), name.c_str());
			}

		private:

			std::unique_ptr<lua_State, void (*)(lua_State *)> state;
	};
}

#endif /* LUAGAME_LUABOX_H_ */
