TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/jdb.cpp \
    src/graphics/drawable.cpp \
    src/utilities/shader_loader.cpp \
    src/utilities/gl3w/gl3w.c \
    src/utilities/tinyxml2/tinyxml2.cpp \
    src/graphics/sprite.cpp

HEADERS += \
    src/jdb.hpp \
    src/graphics/drawable.hpp \
    src/utilities/shader_loader.hpp \
    src/utilities/dependencies.hpp \
    src/utilities/gl3w/gl3w.h \
    src/utilities/gl3w/glcorearb.h \
    src/utilities/tinyxml2/tinyxml2.h \
    src/graphics/sprite.hpp

LIBS += -lGLU -lGL -lSDL2 -lSDL2_image

CXXFLAGS += -Wall -Werrors -pedantic-errors

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
