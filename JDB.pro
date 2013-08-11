TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/jdb.cpp

HEADERS += \
    src/jdb.hpp

LIBS += -lGLEW -lGLU -lSDL2

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11 pedantic
