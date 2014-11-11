CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(luagame)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
SET(luagame_SOURCES
		src/function_wrapper.cpp
		src/luabox.cpp
		src/luagame.cpp
		src/luaerror.cpp
)

FIND_PACKAGE(Lua 5.2 REQUIRED)
FIND_PACKAGE(Boost 1.53 REQUIRED COMPONENTS coroutine system context)

INCLUDE_DIRECTORIES(
	include
	${LUA_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
)

ADD_LIBRARY(luagame ${luagame_SOURCES})

TARGET_LINK_LIBRARIES(luagame
	${LUA_LIBRARIES}
	${Boost_LIBRARIES}
)

ADD_SUBDIRECTORY(tests)
