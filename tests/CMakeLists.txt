set( TEST_SOURCE_DIR "${CMAKE_BINARY_DIR}/tests/src" )

set( TEST_SOURCE_FILES
    "${TEST_SOURCE_DIR}/main.cpp"
    "${TEST_SOURCE_DIR}/utilities/base_line.cpp" )

add_executable( test-sandwiches-game ${SOURCE_FILES} ${TEST_SOURCE_FILES} )
target_link_libraries( test-sandwiches-game "gtest_main;gtest;pthread;${LIBRARIES}" )
