TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += ../src/main.cpp \
    ../src/jdb.cpp \
    ../src/levels/level.cpp \
    ../src/utilities/timer.cpp \
    ../src/dangers/danger_data.cpp \
    ../src/dangers/danger_state.cpp \
    ../src/dangers/danger.cpp \
    ../src/tools/tool.cpp \
    ../src/dangers/player_action_response.cpp \
    ../src/sandwiches/sandwich_data.cpp \
    ../src/sandwiches/sandwich.cpp \
    ../src/utilities/base_line.cpp \
    ../src/dependencies/m2g/src/graphics_library.cpp \
    ../src/dependencies/m2g/src/utilities/tilesets_buffer.cpp \
    ../src/dependencies/m2g/src/text/text_renderer.cpp \
    ../src/dependencies/m2g/src/text/bitmap_font.cpp \
    ../src/dependencies/m2g/src/drawables/tileset.cpp \
    ../src/dependencies/m2g/src/drawables/sprite.cpp \
    ../src/dependencies/m2g/src/drawables/drawable.cpp \
    ../src/dependencies/m2g/src/drawables/animation.cpp \
    ../src/dependencies/m2g/src/drawables/animation_data.cpp \
    ../src/dependencies/m2g/src/dependencies/GL3W/gl3w.c \
    ../src/dependencies/m2g/src/dependencies/tinyxml2/tinyxml2.cpp \
    ../src/dependencies/m2g/src/dependencies/msl/src/shader_loader.cpp \
    ../src/dependencies/m2g/src/drawables/drawables_set.cpp \
    ../src/dependencies/m2g/src/drawables/collidable.cpp \
    ../src/levels/survival_level.cpp \
    ../src/levels/conveyor_belt.cpp \
    ../src/levels/campaign_level.cpp

HEADERS += \
    ../src/jdb.hpp \
    ../src/levels/level.hpp \
    ../src/utilities/timer.hpp \
    ../src/dangers/player_action.hpp \
    ../src/dangers/danger_data.hpp \
    ../src/dangers/danger_state.hpp \
    ../src/dangers/danger.hpp \
    ../src/tools/tool.hpp \
    ../src/dangers/player_action_response.hpp \
    ../src/sandwiches/sandwich_data.hpp \
    ../src/sandwiches/sandwich.hpp \
    ../src/utilities/base_line.hpp \
    ../src/dependencies/m2g/src/graphics_library.hpp \
    ../src/dependencies/m2g/src/utilities/tilesets_buffer.hpp \
    ../src/dependencies/m2g/src/text/text_renderer.hpp \
    ../src/dependencies/m2g/src/text/bitmap_font.hpp \
    ../src/dependencies/m2g/src/drawables/tileset.hpp \
    ../src/dependencies/m2g/src/drawables/sprite.hpp \
    ../src/dependencies/m2g/src/drawables/drawable.hpp \
    ../src/dependencies/m2g/src/drawables/animation.hpp \
    ../src/dependencies/m2g/src/drawables/animation_data.hpp \
    ../src/dependencies/m2g/src/dependencies/GL3W/gl3w.h \
    ../src/dependencies/m2g/src/dependencies/GL3W/gl3.h \
    ../src/dependencies/m2g/src/dependencies/tinyxml2/tinyxml2.h \
    ../src/dependencies/m2g/src/dependencies/msl/src/shader_loader.hpp \
    ../src/dependencies/m2g/src/dependencies/dependencies.hpp \
    ../src/dependencies/m2g/src/drawables/drawables_set.hpp \
    ../src/dependencies/m2g/src/drawables/collidable.hpp \
    ../src/levels/survival_level.hpp \
    ../src/levels/conveyor_belt.hpp \
    ../src/levels/campaign_level.hpp

LIBS += -lGL -lGLU -lSDL2 -lSDL2_image -lSDL2_ttf -lftgl -ldl -lsfml-system -lsfml-audio

CXXFLAGS += -Wall -Werrors -pedantic-errors

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11


win32 {
    debug {
        DESTDIR = ../debug-win32
    }

    INCLUDEPATH += /home/moises/cross-compiling-windows/mxe/usr/include/
    INCLUDEPATH += /home/moises/cross-compiling-windows/mxe/usr/i686-w64-mingw32/include/
    INCLUDEPATH += /home/moises/cross-compiling-windows/mxe/usr/i686-w64-mingw32/include/freetype2/

    message( "Compiling for Windows 32" )

    CXXFLAGS += -std=c++11
    #CXXFLAGS += -std=c++11
}

unix {
    debug {
        DESTDIR = .
    }



    INCLUDEPATH += /usr/include/freetype2/

    message( "Compiling for UNIX" )
}

BUILD_DATA_DIR = $$DESTDIR/.build_data
OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui
