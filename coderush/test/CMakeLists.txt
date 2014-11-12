PROJECT(coderush_test)

SET(coderush_test_SOURCES
	level.cpp
	main.cpp
)

FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Lua 5.2 REQUIRED)

INCLUDE_DIRECTORIES(
	${GTest_INCLUDE_DIR}
	${LUA_INCLUDE_DIR}
	${coderush_SOURCE_DIR}/src
)

ADD_EXECUTABLE(coderush_tests ${coderush_test_SOURCES})

TARGET_LINK_LIBRARIES(coderush_tests
	${GTEST_LIBRARIES}
	coderushcore
	luagame
	${LUA_LIBRARIES}
	pthread
)

ADD_CUSTOM_COMMAND(
	TARGET coderush_tests
	POST_BUILD
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files
		$<TARGET_FILE_DIR:coderush_tests>/files
	DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/files/*")

GTEST_ADD_TESTS(coderush_tests "" ${coderush_test_SOURCES})
