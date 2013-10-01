TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/jdb.cpp \
    src/dependencies/msl/src/shader_loader.cpp \
    src/dependencies/graphics/src/dependencies/tinyxml2/tinyxml2.cpp \
    src/dependencies/graphics/src/drawables/tileset.cpp \
    src/dependencies/graphics/src/drawables/sprite.cpp \
    src/dependencies/graphics/src/drawables/drawable.cpp \
    src/dependencies/GL3W/gl3w.c \
    src/dependencies/graphics/src/utilities/tilesets_buffer.cpp \
    src/dependencies/graphics/src/drawables/animation_data.cpp \
    src/dependencies/graphics/src/drawables/animation.cpp \
    src/dependencies/graphics/src/graphics_loader.cpp \
    src/level.cpp \
    src/utilities/timer.cpp \
    src/dangers/danger_data.cpp \
    src/dangers/danger_state.cpp

HEADERS += \
    src/jdb.hpp \
    src/dependencies/dependencies.hpp \
    src/dependencies/msl/src/shader_loader.hpp \
    src/dependencies/graphics/src/dependencies/tinyxml2/tinyxml2.h \
    src/dependencies/graphics/src/drawables/tileset.hpp \
    src/dependencies/graphics/src/drawables/sprite.hpp \
    src/dependencies/graphics/src/drawables/drawable.hpp \
    src/dependencies/GL3W/gl3w.h \
    src/dependencies/GL3W/gl3.h \
    src/dependencies/graphics/src/utilities/tilesets_buffer.hpp \
    src/dependencies/graphics/src/drawables/animation_data.hpp \
    src/dependencies/graphics/src/drawables/animation.hpp \
    src/dependencies/graphics/src/graphics_loader.hpp \
    src/level.hpp \
    src/utilities/timer.hpp \
    src/dangers/player_action.hpp \
    src/dangers/danger_data.hpp \
    src/dangers/danger_state.hpp

LIBS += -lGL -lGLU -lSDL2 -lSDL2_image

CXXFLAGS += -Wall -Werrors -pedantic-errors

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
