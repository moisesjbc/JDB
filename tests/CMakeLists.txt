set( TEST_SOURCE_DIR "${CMAKE_BINARY_DIR}/tests/src" )

set( TEST_SOURCE_FILES
    "${TEST_SOURCE_DIR}/main.cpp" )

add_executable( test-sandwiches-game ${TEST_SOURCE_FILES} )
target_link_libraries( test-sandwiches-game "gtest_main;gtest;pthread" )
