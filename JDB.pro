TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/jdb.cpp \
    src/libraries/shader_loader.cpp \
    src/libraries/gl3w/gl3w.c \
    src/libraries/tinyxml2/tinyxml2.cpp \
    src/libraries/m2g/src/tileset.cpp \
    src/libraries/m2g/src/sprite.cpp \
    src/libraries/m2g/src/drawable.cpp

HEADERS += \
    src/jdb.hpp \
    src/libraries/shader_loader.hpp \
    src/libraries/dependencies.hpp \
    src/libraries/gl3w/gl3w.h \
    src/libraries/gl3w/glcorearb.h \
    src/libraries/tinyxml2/tinyxml2.h \
    src/libraries/m2g/src/tileset.hpp \
    src/libraries/m2g/src/sprite.hpp \
    src/libraries/m2g/src/drawable.hpp

LIBS += -lGLU -lGL -lSDL2 -lSDL2_image

CXXFLAGS += -Wall -Werrors -pedantic-errors

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11
