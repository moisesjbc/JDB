set( TEST_SOURCE_DIR "${CMAKE_BINARY_DIR}/tests/src" )

set( TEST_SOURCE_FILES
    "${TEST_SOURCE_DIR}/main.cpp"
    "${TEST_SOURCE_DIR}/utilities/base_line.cpp"
    "${TEST_SOURCE_DIR}/utilities/timer.cpp"
    "${TEST_SOURCE_DIR}/dangers/dangers_counter.cpp"
    "${TEST_SOURCE_DIR}/dangers/danger_data_parser.cpp"
    "${TEST_SOURCE_DIR}/profiles/profile.cpp"
    "${TEST_SOURCE_DIR}/profiles/profile_json_parser.cpp" )

add_executable( test-sandwiches-game ${SOURCE_FILES} ${TEST_SOURCE_FILES} )
target_link_libraries( test-sandwiches-game "gtest_main;gtest;pthread;${LIBRARIES}" )
