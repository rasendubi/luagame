CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(coderush)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Werror")
SET(coderush_SOURCES
	src/level.cpp
)

FIND_PACKAGE(Lua 5.2 REQUIRED)

INCLUDE_DIRECTORIES(
	${LUA_INCLUDE_DIR}
	${luagame_SOURCE_DIR}/include
)

ADD_LIBRARY(coderushcore ${coderush_SOURCES})
ADD_EXECUTABLE(coderush src/main.cpp ${coderush_SOURCES})

TARGET_LINK_LIBRARIES(coderush
	luagame
	${LUA_LIBRARIES}
)

ADD_SUBDIRECTORY(test)
