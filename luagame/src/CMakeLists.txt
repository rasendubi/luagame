SET(SRC_FILES
		function_wrapper.cpp
		luabox.cpp
		luagame.cpp
		luaerror.cpp
)

FIND_PACKAGE(Lua 5.2 REQUIRED)

INCLUDE_DIRECTORIES(
	${LUA_INCLUDE_DIR}
)

ADD_LIBRARY(luagame ${SRC_FILES})

TARGET_LINK_LIBRARIES(luagame
	${LUA_LIBRARIES}
)
