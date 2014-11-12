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
	 * Mask type for setting hook.
	 */
	enum class Luamask : int {
		None = 0,
		/**
		 * The hook is called just after Lua enters the new function,
		 * before the function gets its arguments.
		 *
		 * \see LUA_MASKCALL
		 */
		Call = LUA_MASKCALL,
		/**
		 * The hook is called just before Lua leaves the function.
		 *
		 * \see LUA_MASKRET
		 */
		Return = LUA_MASKRET,
		/**
		 * The hook is called when the interpreter is about to start
		 * the execution of a new line of code, or when it jumps back
		 * in the code (even to the same line).
		 *
		 * \note This event only happens while Lua is executing a Lua function.
		 * \see LUA_MASKLINE
		 */
		Line = LUA_MASKLINE,
		/**
		 * is called after the interpreter executes every count instructions.
		 *
		 * \note This event only happens while Lua is executing a Lua function.
		 * \see LUA_MASKCOUNT
		 */
		Count = LUA_MASKCOUNT,
	};

	inline Luamask operator|(Luamask a, Luamask b) {
		return static_cast<Luamask>(static_cast<int>(a) | static_cast<int>(b));
	}

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

			/**
			 * Loads specified file on top of lua state.
			 *
			 * \see Luabox::run()
			 * \see Luabox::set_file()
			 */
			Luabox(const std::string& filename) : Luabox() {
				set_file(filename);
			}

			Luabox(const Luabox&) = delete;
			Luabox& operator=(const Luabox&) = delete;

			Luabox(Luabox&&) = default;
			Luabox& operator=(Luabox&&) = default;

			void run_file(const std::string& filename);

			/**
			 * Loads given file and places it on top of stack as function.
			 *
			 * \warning Be sure to not leave other values on top of function
			 *     before calling \c run() (unless you now what you're doing).
			 *
			 * \see Luabox::run()
			 */
			void set_file(const std::string& filename);

			/**
			 * Executes function placed on top of the stack.
			 *
			 * \see Luabox::set_file()
			 * \see Luabox::run_file()
			 */
			void run();

			void open_lib(const char *name, openlib_func lib);

			/**
			 * Registers usual Lua function under \p name name.
			 */
			void register_function(const std::string& name, lua_CFunction function);

			/**
			 * Registers C++ functor.
			 *
			 * This function automatically gets parameters for your function from the Lua
			 * stack and places results back.
			 *
			 * This template specification is for functors that have destructor. And prepares
			 * proper metatable for garbage collector.
			 *
			 * \note The functor is placed on Lua stack and is managed by Lua. So that,
			 *     the functor should have move or copy constructor.
			 */
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

			/**
			 * Registers C++ functor.
			 *
			 * This function automatically gets parameters for your function from the Lua
			 * stack and places results back.
			 *
			 * This template specification is for PODs and doesn't allocate metatable for
			 * functor. But functor itself is still placed on the Lua stack.
			 */
			template<typename F>
			typename std::enable_if<std::is_pod<F>::value && !std::is_pointer<F>::value, void>::type
					register_function(const std::string& name, F function) {
				using Luagame_impl::function_wrapper;
				void *data = lua_newuserdata(state.get(), sizeof(F));
				new(data) F(function);

				lua_pushcclosure(state.get(), function_wrapper<F>::caller, 1);
				lua_setglobal(state.get(), name.c_str());
			}

			/**
			 * Registers C++ function.
			 *
			 * This function automatically gets parameters for your function from the Lua
			 * stack and places results back.
			 *
			 * This template specification is for pointers and just stores the pointer
			 * itself as light user data.
			 */
			template<typename F>
			typename std::enable_if<std::is_pod<F>::value && std::is_pointer<F>::value, void>::type
					register_function(const std::string& name, F function) {
				using Luagame_impl::function_wrapper;
				lua_pushlightuserdata(state.get(), reinterpret_cast<void *>(function));
				lua_pushcclosure(state.get(), function_wrapper<F>::caller, 1);
				lua_setglobal(state.get(), name.c_str());
			}

			/**
			 * Places value in the global table.
			 */
			template<typename T>
			void register_value(const std::string& name, T value) {
				Luagame_impl::put_value(state.get(), value);
				lua_setglobal(state.get(), name.c_str());
			}

			/**
			 * Sets a function which will be called on every occurrence of given event.
			 *
			 * \param count used only with \c Luamask::Count. Hook will be called after
			 *     every \p count instructions.
			 *
			 * \see Luamask
			 */
			void set_hook(lua_Hook func, Luamask mask, int count = 0) {
				lua_sethook(state.get(), func, static_cast<int>(mask), count);
			}

			template<typename T>
			T get(const std::string& name) {
				lua_getglobal(state.get(), name.c_str());
				T result = Luagame_impl::get_arg<T>(state.get(), -1);
				lua_pop(state.get(), 1);
				return result;
			}

		private:

			std::unique_ptr<lua_State, void (*)(lua_State *)> state;
	};

}

#endif /* LUAGAME_LUABOX_H_ */
