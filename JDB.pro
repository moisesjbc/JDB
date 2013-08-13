TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/jdb.cpp \
    src/graphics/drawable.cpp \
    src/utilities/shader_loader.cpp \
    src/graphics/sprite.cpp

HEADERS += \
    src/jdb.hpp \
    src/graphics/drawable.hpp \
    src/utilities/shader_loader.hpp \
    src/utilities/dependencies.hpp \
    src/graphics/sprite.hpp

LIBS += -lGLEW -lGLU -lGL -lSDL2

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11 pedantic
