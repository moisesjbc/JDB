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
    ../src/levels/survival_level.cpp \
    ../src/levels/conveyor_belt.cpp \
    ../src/levels/campaign_level.cpp \
    ../src/game_states/game_state.cpp \
    ../src/game_states/main_menu.cpp \
    ../src/utilities/window.cpp \
    ../src/utilities/sound_manager.cpp

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
    ../src/levels/survival_level.hpp \
    ../src/levels/conveyor_belt.hpp \
    ../src/levels/campaign_level.hpp \
    ../src/game_states/game_state.hpp \
    ../src/utilities/window.hpp \
    ../src/game_states/main_menu.hpp \
    ../src/utilities/sound_manager.hpp

LIBS += -lm2g -lSDL2 -lSDL2_image -lSDL2_ttf -ldl -lsfml-system -lsfml-audio -ltinyxml2
LIBS += -lboost_system -lboost_filesystem

CXXFLAGS += -Wall -Werrors -pedantic-errors

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11

INCLUDEPATH += ../src

DESTDIR = .
BUILD_DATA_DIR = $$DESTDIR/.build_data
OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui
