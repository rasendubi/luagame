SET(SRC_FILES
		function_wrapper.cpp
		luabox.cpp
		luagame.cpp
		luaerror.cpp
)

FIND_PACKAGE(Lua 5.2 REQUIRED)
FIND_PACKAGE(Boost 1.53 REQUIRED COMPONENTS coroutine system context)

INCLUDE_DIRECTORIES(
	${LUA_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
)

ADD_LIBRARY(luagame ${SRC_FILES})

TARGET_LINK_LIBRARIES(luagame
	${LUA_LIBRARIES}
	${Boost_LIBRARIES}
)
