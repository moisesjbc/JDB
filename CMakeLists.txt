cmake_minimum_required( VERSION 2.8 )
set( CMAKE_BUILD_TYPE Debug )

# Project info
project( jdb )
set( PROJECT_VERSION_MAJOR 0 )
set( PROJECT_VERSION_MINOR 1 )
set( PROJECT_VERSION_PATCH 0 )
set( PROJECT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )

# Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected, adding compile flags")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -pedantic-errors" )
endif(CMAKE_COMPILER_IS_GNUCXX)

# Common libraries
include( FindPkgConfig )
set( LIBRARIES "${LIBRARIES};tinyxml2" )
set( LIBRARIES "${LIBRARIES};-lm2g -ldl -ltgui -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lboost_system -lboost_filesystem" )

set( SOURCE_DIR "${CMAKE_BINARY_DIR}/src" )

include_directories( "${SOURCE_DIR}" )

set( SOURCE_FILES
    "${SOURCE_DIR}/main.cpp"
    "${SOURCE_DIR}/jdb.cpp"
    "${SOURCE_DIR}/levels/level.cpp"
    "${SOURCE_DIR}/dangers/danger_data.cpp"
    "${SOURCE_DIR}/dangers/danger_state.cpp"
    "${SOURCE_DIR}/dangers/danger.cpp"
    "${SOURCE_DIR}/tools/tool.cpp"
    "${SOURCE_DIR}/dangers/player_action_response.cpp"
    "${SOURCE_DIR}/sandwiches/sandwich_data.cpp"
    "${SOURCE_DIR}/sandwiches/sandwich.cpp"
    "${SOURCE_DIR}/utilities/base_line.cpp"
    "${SOURCE_DIR}/levels/survival_level.cpp"
    "${SOURCE_DIR}/levels/conveyor_belt.cpp"
    "${SOURCE_DIR}/levels/campaign_level.cpp"
    "${SOURCE_DIR}/game_states/game_state.cpp"
    "${SOURCE_DIR}/game_states/main_menu.cpp"
    "${SOURCE_DIR}/utilities/sound_manager.cpp"
    "${SOURCE_DIR}/game_states/pause_menu.cpp"
    "${SOURCE_DIR}/game_states/level_intro.cpp"
    "${SOURCE_DIR}/utilities/level_book.cpp"
    "${SOURCE_DIR}/dangers/dangers_file_parser.cpp"
    "${SOURCE_DIR}/utilities/texture_picture.cpp"
)


add_executable( "${PROJECT_NAME}" ${SOURCE_FILES} )
target_link_libraries( "bin/${PROJECT_NAME}" ${LIBRARIES} )

