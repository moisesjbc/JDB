cmake_minimum_required( VERSION 2.8 )
set( CMAKE_BUILD_TYPE Release )

# Project info
project( sandwiches-game )
set( PROJECT_VERSION_MAJOR 1 )
set( PROJECT_VERSION_MINOR 1 )
set( PROJECT_VERSION_PATCH 0 )
set( PROJECT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )

set( DONT_EDIT_THIS_FILE_MESSAGE "This file is generated automatically by CMakeLists.txt every time cmake is run. Don't edit it!" )

# Compiler flags
if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected, adding compile flags")
        add_compile_options( "-std=c++11" "-Wall" "-Werror" "-pedantic-errors" )
    if( BUILD_USING_MXE )
        add_compile_options( "-DSFML_STATIC" )
    endif()
endif(CMAKE_COMPILER_IS_GNUCXX)

# Disable logs in release mode.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message("Disabling logs")
    add_compile_options( "-DELPP_DISABLE_LOGS" )
endif()

# Common libraries
link_directories("${CMAKE_SOURCE_DIR}/third-party/ubuntu_14_04_x86_64/lib")
include_directories("${CMAKE_SOURCE_DIR}/third-party/ubuntu_14_04_x86_64/include")
include( FindPkgConfig )
if( BUILD_USING_MXE )
    set( LIBRARIES "${LIBRARIES};tinyxml2;tgui-s;sfml-graphics-s;sfml-window-s;sfml-audio-s;sfml-system-s;opengl32;freetype;winmm;bz2;m2g;opengl32;freetype;jpeg;winmm;gdi32;openal32;FLAC;vorbisenc;vorbisfile;vorbis;ogg;bz2;png;z;m;boost_system-mt;boost_filesystem-mt;dinput8;dxguid;dxerr8;user32;gdi32;winmm;imm32;ole32;oleaut32;shell32;version;uuid;harfbuzz;glib-2.0;rtutils;intl;iconv;winmm;ws2_32;sndfile" )
    #openal32;flac
else( BUILD_USING_MXE )
    set( LIBRARIES "${LIBRARIES};tinyxml2;m2g;tgui;sfml-graphics;sfml-window;sfml-audio;sfml-system;boost_system;boost_filesystem" )
endif( BUILD_USING_MXE )

set( SOURCE_DIR "${CMAKE_BINARY_DIR}/src" )
set( INCLUDE_DIR "${CMAKE_BINARY_DIR}/include" )

include_directories( "${INCLUDE_DIR}" )

# main_menu.cpp displays the game version, so we update it here.
configure_file(
    "${INCLUDE_DIR}/utilities/version_string.hpp.in"
    "${INCLUDE_DIR}/utilities/version_string.hpp"
)

set( SOURCE_FILES
    "${SOURCE_DIR}/jdb.cpp"
    "${SOURCE_DIR}/levels/level.cpp"
    "${SOURCE_DIR}/dangers/danger_data.cpp"
    "${SOURCE_DIR}/dangers/danger_state.cpp"
    "${SOURCE_DIR}/dangers/danger.cpp"
    "${SOURCE_DIR}/dangers/dangers_counter.cpp"
    "${SOURCE_DIR}/dangers/danger_data_parser.cpp"
    "${SOURCE_DIR}/tools/tool.cpp"
    "${SOURCE_DIR}/dangers/player_action_response.cpp"
    "${SOURCE_DIR}/sandwiches/sandwiches_manager.cpp"
    "${SOURCE_DIR}/sandwiches/sandwich_data.cpp"
    "${SOURCE_DIR}/sandwiches/sandwich.cpp"
    "${SOURCE_DIR}/dangers/danger_mutation.cpp"
    "${SOURCE_DIR}/utilities/base_line.cpp"
    "${SOURCE_DIR}/levels/survival_level.cpp"
    "${SOURCE_DIR}/levels/conveyor_belt.cpp"
    "${SOURCE_DIR}/levels/campaign_level.cpp"
    "${SOURCE_DIR}/game_states/game_state.cpp"
    "${SOURCE_DIR}/game_states/credits_screen.cpp"
    "${SOURCE_DIR}/profiles/profile.cpp"
    "${SOURCE_DIR}/profiles/profile_json_parser.cpp"
    "${SOURCE_DIR}/game_states/gui_menu.cpp"
    "${SOURCE_DIR}/game_states/main_menu.cpp"
    "${SOURCE_DIR}/game_states/campaign_level_selection_menu.cpp"
    "${SOURCE_DIR}/game_states/game_over_screen.cpp"
    "${SOURCE_DIR}/utilities/sound_manager.cpp"
    "${SOURCE_DIR}/game_states/pause_menu.cpp"
    "${SOURCE_DIR}/game_states/end_of_campaign_screen.cpp"
    "${SOURCE_DIR}/game_states/level_intro.cpp"
    "${SOURCE_DIR}/utilities/level_book.cpp"
    "${SOURCE_DIR}/utilities/texture_picture.cpp"
    "${SOURCE_DIR}/utilities/volume_control_panel.cpp"
    "${SOURCE_DIR}/utilities/unicode_file_reader.cpp"
    "${SOURCE_DIR}/level_ui/level_ui.cpp"
    "${SOURCE_DIR}/level_ui/campaign_level_ui.cpp"
    "${SOURCE_DIR}/level_ui/survival_level_ui.cpp"
    "${SOURCE_DIR}/utilities/timer.cpp"
)

# Workaround for showing include files in Qtcreator.
file( GLOB_RECURSE HEADER_FILES "include/*.hpp" )
file( GLOB_RECURSE HEADER_TEMPLATE_FILES "include/*.hpp.in" )
add_custom_target( HEADERS SOURCES ${HEADER_FILES} ${HEADER_TEMPLATE_FILES} )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
set( EXEC_NAME "${PROJECT_NAME}" )
if( NOT BUILD_USING_MXE )
    add_executable( "${EXEC_NAME}" "${SOURCE_DIR}/main.cpp" ${SOURCE_FILES} )
else()
    add_executable( "${EXEC_NAME}" WIN32 "${SOURCE_DIR}/main.cpp" ${SOURCE_FILES} )
endif()
target_link_libraries( "${EXEC_NAME}" ${LIBRARIES} )

if( NOT BUILD_USING_MXE )
    include(${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
endif()

# Instalation
if( NOT BUILD_USING_MXE )
    set( CMAKE_INSTALL_PREFIX /opt/${PROJECT_NAME} )
else()
    set( CMAKE_INSTALL_PREFIX / )
endif()
install( TARGETS "${EXEC_NAME}" RUNTIME DESTINATION bin )
install( DIRECTORY share/${PROJECT_NAME} DESTINATION share PATTERN "*~" EXCLUDE PATTERN profile.json EXCLUDE )
install( FILES share/${PROJECT_NAME}/savegames/profile.json DESTINATION share/${PROJECT_NAME}/savegames/
         PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
         GROUP_READ GROUP_WRITE GROUP_EXECUTE
         WORLD_READ WORLD_WRITE WORLD_EXECUTE )
if( NOT BUILD_USING_MXE )
    file( WRITE ${PROJECT_NAME}-launcher "#!/bin/bash
(cd \"${CMAKE_INSTALL_PREFIX}/bin\" && export LD_LIBRARY_PATH=. && ./${EXEC_NAME})
")
    install( PROGRAMS ${PROJECT_NAME}-launcher DESTINATION /usr/local/bin RENAME ${EXEC_NAME} )
    install( DIRECTORY "${CMAKE_SOURCE_DIR}/third-party/ubuntu_14_04_x86_64/lib/" DESTINATION bin )
    install( FILES ${CMAKE_SOURCE_DIR}/share/applications/${EXEC_NAME}.desktop DESTINATION /usr/share/applications )
else()
    install( DIRECTORY "${CMAKE_SOURCE_DIR}/third-party/win32/bin/" DESTINATION bin )
endif()



# Packaging
set( CPACK_PACKAGE_NAME ${PROJECT_NAME} )
if( WIN32 )
    set( CPACK_GENERATOR "ZIP" )
else()
    set( CPACK_GENERATOR "DEB" )
endif()
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Sandwiches Game is a 2D ability game"  )
set( CPACK_PACKAGE_CONTACT "Moisés J. Bonilla Caraballo <moisesjbc@gmail.com>" )
set( CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH} )
set( CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libtinyxml2-0.0.0" )
if( WIN32 )
    set( CPACK_PACKAGING_INSTALL_PREFIX "/")
else()
    set( CPACK_PACKAGING_INSTALL_PREFIX "/opt/${CPACK_PACKAGE_NAME}")
endif()
set( CPACK_SET_DESTDIR true )
set( CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
set( CPACK_STRIP_FILES true )
include( CPack )

if( NOT WIN32 )
    configure_file(
        ${CMAKE_SOURCE_DIR}/share/applications/${EXEC_NAME}.desktop.in
        ${CMAKE_SOURCE_DIR}/share/applications/${EXEC_NAME}.desktop
    )
endif()
