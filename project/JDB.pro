TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/jdb.cpp \
    src/dependencies/msl/src/shader_loader.cpp \
    src/dependencies/graphics/src/library.cpp \
    src/dependencies/graphics/src/dependencies/gpu-buffers/src/rects_buffer.cpp \
    src/dependencies/graphics/src/dependencies/gpu-buffers/src/entities_buffer.cpp \
    src/dependencies/graphics/src/dependencies/gpu-buffers/src/buffer.cpp \
    src/dependencies/graphics/src/dependencies/tinyxml2/tinyxml2.cpp \
    src/dependencies/graphics/src/drawables/tileset.cpp \
    src/dependencies/graphics/src/drawables/sprite.cpp \
    src/dependencies/graphics/src/drawables/drawable.cpp \
    src/dependencies/GL3W/gl3w.c

HEADERS += \
    src/jdb.hpp \
    src/dependencies/dependencies.hpp \
    src/dependencies/msl/src/shader_loader.hpp \
    src/dependencies/graphics/src/library.hpp \
    src/dependencies/graphics/src/dependencies/gpu-buffers/src/rects_buffer.hpp \
    src/dependencies/graphics/src/dependencies/gpu-buffers/src/entities_buffer.hpp \
    src/dependencies/graphics/src/dependencies/gpu-buffers/src/buffer.hpp \
    src/dependencies/graphics/src/dependencies/tinyxml2/tinyxml2.h \
    src/dependencies/graphics/src/drawables/tileset.hpp \
    src/dependencies/graphics/src/drawables/sprite.hpp \
    src/dependencies/graphics/src/drawables/drawable.hpp \
    src/dependencies/GL3W/gl3w.h \
    src/dependencies/GL3W/gl3.h

LIBS += -lGL -lGLU -lSDL2 -lSDL2_image

CXXFLAGS += -Wall -Werrors -pedantic-errors

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
