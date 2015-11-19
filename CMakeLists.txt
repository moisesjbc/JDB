cmake_minimum_required( VERSION 2.8 )
set( CMAKE_BUILD_TYPE Release )

# Project info
project( sandwiches-game )
set( PROJECT_VERSION_MAJOR 0 )
set( PROJECT_VERSION_MINOR 1 )
set( PROJECT_VERSION_PATCH 0 )
set( PROJECT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )

# Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected, adding compile flags")
        add_compile_options( "-std=c++11" "-Wall" "-Werror" "-pedantic-errors" )
endif(CMAKE_COMPILER_IS_GNUCXX)

# Common libraries
link_directories("${CMAKE_SOURCE_DIR}/third-party/lib")
include_directories("${CMAKE_SOURCE_DIR}/third-party/include")
message("${CMAKE_SOURCE_DIR}/third-party/lib")
include( FindPkgConfig )
set( LIBRARIES "${LIBRARIES};tinyxml2" )
set( LIBRARIES "${LIBRARIES};m2g;tgui;sfml-graphics;sfml-window;sfml-audio;sfml-system;boost_system;boost_filesystem" )

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
    "${SOURCE_DIR}/utilities/volume_control_panel.cpp"
)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( EXEC_NAME "${PROJECT_NAME}" )
add_executable( "${EXEC_NAME}" ${SOURCE_FILES} )
target_link_libraries( "${EXEC_NAME}" ${LIBRARIES} )

# Instalation
set( CMAKE_INSTALL_PREFIX /opt/${PROJECT_NAME} )
install( TARGETS "${EXEC_NAME}" RUNTIME DESTINATION bin )
install( DIRECTORY share/${PROJECT_NAME} DESTINATION share )
file( WRITE ${PROJECT_NAME}-launcher "#!/bin/bash
(cd \"${CMAKE_INSTALL_PREFIX}/bin\" && ./${EXEC_NAME})
")
install( PROGRAMS ${PROJECT_NAME}-launcher DESTINATION /usr/local/bin RENAME ${EXEC_NAME} )
install( DIRECTORY "${CMAKE_SOURCE_DIR}/third-party/lib/" DESTINATION bin )

# Packaging
set( CPACK_PACKAGE_NAME ${PROJECT_NAME} )
set( CPACK_GENERATOR "DEB" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Sandwiches Game is a 2D ability game"  )
set( CPACK_PACKAGE_CONTACT "Moisés J. Bonilla Caraballo" )
set( CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH} )
set( CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )
set( CPACK_PACKAGING_INSTALL_PREFIX "/opt/${CPACK_PACKAGE_NAME}")
set( CPACK_SET_DESTDIR true )
set( CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
include( CPack )
