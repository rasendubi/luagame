SET(TEST_SOURCES
	luabox.cpp
	main.cpp
)

FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Lua 5.2 REQUIRED)

INCLUDE_DIRECTORIES(
	${GTest_INCLUDE_DIR}
	${LUA_INCLUDE_DIR}
)

ADD_EXECUTABLE(tests ${TEST_SOURCES})

TARGET_LINK_LIBRARIES(tests
	luagame
	${GTEST_LIBRARIES}
)

ADD_CUSTOM_COMMAND(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/files"
	COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/files
	$<TARGET_FILE_DIR:tests>/files
	DEPENDS  "${CMAKE_CURRENT_SOURCE_DIR}/files/*")

ADD_CUSTOM_TARGET(run_tests
	COMMAND tests
	DEPENDS tests ${CMAKE_CURRENT_BINARY_DIR}/files
	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

GTEST_ADD_TESTS(tests "" ${TEST_SOURCES})