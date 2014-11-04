#ifndef LUAGAME_FUNCTION_CALLER_IMPL_H_
#define LUAGAME_FUNCTION_CALLER_IMPL_H_

extern "C" {
#include <lualib.h>
}

#include "function_traits.h"

namespace Luagame_impl {
	template<typename Arg>
	Arg get_arg(lua_State *state, int n);

	/* Puts value on lua stack and return number of values */
	template<typename R>
	int put_value(lua_State *state, R result);

	template<typename F, int N>
	struct tuple_builder;

	template<typename F>
	class function_caller {
			using traits = function_traits<F>;
			using R = typename traits::return_type;
		public:

			static R call(F* func, lua_State *state) {
				return call_helper< traits::arity >::engage(func,
						tuple_builder<F, traits::arity>::build(state));
			}

		private:

			using params_tuple = decltype(tuple_builder<F, traits::arity>::build(nullptr));

			template<int N, int... Rest>
			struct call_helper {
				static R engage(F *func, params_tuple params) {
					return call_helper<N-1, N-1, Rest...>::engage(func, params);
				}

			};

			template<int... I>
			struct call_helper<0, I...> {
				static R engage(F *func, params_tuple params) {
					(void)params; // Remove error if params is empty
					return (*func)(std::get<I>(params)...);
				}
			};
	};

	template<typename F, typename R>
	struct processor {
		static int process(lua_State *state, F* function) {
			return put_value(state, function_caller<F>::call(function, state));
		}
	};

	template<typename F>
	struct processor<F, void> {
		static int process(lua_State *state, F* function) {
			function_caller<F>::call(function, state);
			return 0;
		}
	};

	template<typename F, typename = void>
	struct function_wrapper;

	template<typename F>
	struct function_wrapper<F, typename std::enable_if<std::is_pointer<F>::value>::type> {
		static int caller(lua_State *state) {
			using R = typename function_traits<F>::return_type;
			F function = reinterpret_cast<F>(lua_touserdata(state, lua_upvalueindex(1)));
			return processor<typename std::remove_pointer<F>::type, R>::process(state, function);
		}
	};

	template<typename F>
	struct function_wrapper<F, typename std::enable_if<!std::is_pointer<F>::value>::type> {
		static int caller(lua_State *state) {
			using R = typename function_traits<F>::return_type;
			F *function = reinterpret_cast<F*>(lua_touserdata(state, lua_upvalueindex(1)));
			return processor<F, R>::process(state, function);
		}

		static int destroyer(lua_State *state) {
			F *data = reinterpret_cast<F*>(lua_touserdata(state, 1));
			data->~F();
			return 0;
		}
	};


	template<typename F, int N>
	struct tuple_builder {
		static constexpr auto i = function_traits<F>::arity - N;
		using original_type = typename function_traits<F>::template argument<i>::type;
		using type = typename std::remove_const<typename std::remove_reference<original_type>::type>::type;

		static auto build(lua_State *state) ->
				decltype(std::tuple_cat(std::tuple<type>(), tuple_builder<F, N - 1>::build(state))) {

			auto first = std::make_tuple(get_arg<type>(state, i + 1));
			auto rest = tuple_builder<F, N - 1>::build(state);
			return std::tuple_cat(first, rest);
		}
	};

	template<typename F>
	struct tuple_builder<F, 0> {
		static std::tuple<> build(lua_State *) {
			return std::tuple<>();
		}
	};
}

#endif /* LUAGAME_FUNCTION_CALLER_IMPL_H_ */
