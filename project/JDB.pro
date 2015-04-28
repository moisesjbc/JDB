TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += ../src/main.cpp \
    ../src/jdb.cpp \
    ../src/levels/level.cpp \
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
    ../src/utilities/sound_manager.cpp \
    ../src/game_states/pause_menu.cpp \
    ../src/game_states/level_intro.cpp \
    ../src/utilities/level_book.cpp \
    ../src/dangers/dangers_file_parser.cpp

HEADERS += \
    ../src/jdb.hpp \
    ../src/levels/level.hpp \
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
    ../src/game_states/main_menu.hpp \
    ../src/utilities/sound_manager.hpp \
    ../src/game_states/pause_menu.hpp \
    ../src/game_states/level_intro.hpp \
    ../src/utilities/level_book.hpp \
    ../src/dangers/dangers_file_parser.hpp

LIBS += -lm2g -ldl -ltgui -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -ltinyxml2
LIBS += -lboost_system -lboost_filesystem

CXXFLAGS += -Wall -Werrors -pedantic-errors -std=c++11

# http://qt-project.org/forums/viewthread/19989
CONFIG += c++11

INCLUDEPATH += ../src

DESTDIR = .
BUILD_DATA_DIR = $$DESTDIR/.build_data
OBJECTS_DIR = $$BUILD_DATA_DIR/obj
MOC_DIR = $$BUILD_DATA_DIR/moc
RCC_DIR = $$BUILD_DATA_DIR/qrc
UI_DIR = $$BUILD_DATA_DIR/ui
