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
    if( BUILD_USING_MXE=1 )
        add_compile_options( "-march=i686" "-static-libgcc" "-static-libstdc++" )
    endif( BUILD_USING_MXE=1 )
endif(CMAKE_COMPILER_IS_GNUCXX)

# Common libraries
link_directories("${CMAKE_SOURCE_DIR}/third-party/lib")
include_directories("${CMAKE_SOURCE_DIR}/third-party/include")
message("${CMAKE_SOURCE_DIR}/third-party/lib")
include( FindPkgConfig )
if( BUILD_USING_MXE=1 )
    set( LIBRARIES "${LIBRARIES};tinyxml2;m2g;tgui-s;sfml-audio-s;sfml-graphics-s;sfml-window-s;sfml-system-s;opengl32;freetype;jpeg;winmm;gdi32;vorbisenc;vorbisfile;vorbis;ogg;bz2;png;z;m;boost_system-mt;boost_filesystem-mt" )
    set( LIBRARIES "${LIBRARIES};dinput8;dxguid;dxerr8;user32;gdi32;winmm;imm32;ole32;oleaut32;shell32;version;uuid;harfbuzz;glib-2.0" )
    #openal32;flac
else( BUILD_USING_MXE=1 )
    set( LIBRARIES "${LIBRARIES};tinyxml2;m2g;tgui;sfml-graphics;sfml-window;sfml-audio;sfml-system;boost_system;boost_filesystem" )
endif( BUILD_USING_MXE=1 )

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
    "${SOURCE_DIR}/game_states/credits_screen.cpp"
    "${SOURCE_DIR}/game_states/main_menu.cpp"
    "${SOURCE_DIR}/game_states/game_over_screen.cpp"
    "${SOURCE_DIR}/utilities/sound_manager.cpp"
    "${SOURCE_DIR}/game_states/pause_menu.cpp"
    "${SOURCE_DIR}/game_states/end_of_demo_screen.cpp"
    "${SOURCE_DIR}/game_states/level_intro.cpp"
    "${SOURCE_DIR}/utilities/level_book.cpp"
    "${SOURCE_DIR}/dangers/dangers_file_parser.cpp"
    "${SOURCE_DIR}/utilities/texture_picture.cpp"
    "${SOURCE_DIR}/utilities/volume_control_panel.cpp"
    "${SOURCE_DIR}/utilities/unicode_file_reader.cpp"
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
(cd \"${CMAKE_INSTALL_PREFIX}/bin\" && export LD_LIBRARY_PATH=. && ./${EXEC_NAME})
")
install( PROGRAMS ${PROJECT_NAME}-launcher DESTINATION /usr/local/bin RENAME ${EXEC_NAME} )
install( DIRECTORY "${CMAKE_SOURCE_DIR}/third-party/lib/" DESTINATION bin )
install( FILES ${CMAKE_SOURCE_DIR}/share/applications/${EXEC_NAME}.desktop DESTINATION /usr/share/applications )

# Packaging
set( CPACK_PACKAGE_NAME ${PROJECT_NAME} )
set( CPACK_GENERATOR "DEB" )
set( CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Sandwiches Game is a 2D ability game"  )
set( CPACK_PACKAGE_CONTACT "Moisés J. Bonilla Caraballo <moisesjbc@gmail.com>" )
set( CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH} )
set( CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}" )
set( CPACK_DEBIAN_PACKAGE_DEPENDS "libtinyxml2-0.0.0" )
set( CPACK_PACKAGING_INSTALL_PREFIX "/opt/${CPACK_PACKAGE_NAME}")
set( CPACK_SET_DESTDIR true )
set( CPACK_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX} )
set( CPACK_STRIP_FILES true )
include( CPack )

configure_file(
    ${CMAKE_SOURCE_DIR}/share/applications/${EXEC_NAME}.desktop.in
    ${CMAKE_SOURCE_DIR}/share/applications/${EXEC_NAME}.desktop
)
