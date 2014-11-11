project(luagame_test)
ADD_SUBDIRECTORY(maze)

SET(TEST_SOURCES
	boost.cpp
	luabox.cpp
	luagame.cpp
	main.cpp
)

FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Lua 5.2 REQUIRED)
FIND_PACKAGE(Boost 1.53 REQUIRED COMPONENTS coroutine system context)

INCLUDE_DIRECTORIES(
	${GTest_INCLUDE_DIR}
	${LUA_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
)

ADD_EXECUTABLE(luagame_tests ${TEST_SOURCES})

TARGET_LINK_LIBRARIES(luagame_tests
	luagame
	${GTEST_LIBRARIES}
	${Boost_LIBRARIES}
	pthread
)

ADD_CUSTOM_COMMAND(
	TARGET luagame_tests
	POST_BUILD
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files
		$<TARGET_FILE_DIR:luagame_tests>/files
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/files/*")

GTEST_ADD_TESTS(luagame_tests "" ${TEST_SOURCES})
