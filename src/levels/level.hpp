/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

    This file is part of JDB.

    JDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JDB.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <SDL2/SDL.h>
#include "../sandwiches/sandwich.hpp"
#include "../utilities/timer.hpp"
#include "../dangers/danger.hpp"
#include "../tools/tool.hpp"
#include "conveyor_belt.hpp"
#include <m2g/drawables/drawables_set.hpp>
#include <m2g/text/text_renderer.hpp>
#include <game_states/game_state.hpp>

namespace jdb {

const unsigned int N_SANDWICHES = 5;

class Level : public GameState
{
    public:
        /***
         * 1. Construction
         ***/
        Level( Window& window, SoundManager* soundManager, unsigned int levelIndex );


        /***
         * 2. Destruction
         ***/
        virtual ~Level();


    protected:
        /***
         * 3. Level Loading
         ***/
        virtual void load( unsigned int levelIndex ) = 0;
        void loadSandwichData();
        void loadDangerData();


        /***
         * 4. Main loop
         ***/
        void handleUserInput( const SDL_Event& event, Sandwich** sandwiches );
        virtual bool finishPredicate() const = 0;
        virtual void resetTimer() = 0;


        /***
         * 5. Auxiliar methods
         ***/
        void drawTimer( int time );
        int getSeconds() const ;


        /***
         * 6. GameState interface
         ***/
        virtual void init();
        virtual void handleEvents();
        virtual void update();
        virtual void draw();
        virtual void pause(){}
        virtual void resume(){}


    private:
        /***
         * 7. Initialization
         ***/
        void initGUI();


    protected:
        /***
         * Attributes
         ***/
        Timer timer_;

        ConveyorBelt conveyorBelt_;

        tinyxml2::XMLDocument xmlFile;

        m2g::GraphicsLibrary graphicsLibrary_;

        std::vector< DangerDataPtr > dangerData;
        std::vector< SandwichDataPtr > sandwichData;

        std::mutex coutMutex;

        // Players tool
        ToolPtr tool_;

        // Jacob's life
        int jacobHp_;

        // GUI sprites.
        m2g::DrawablesSet guiSprites_;
        m2g::SpritePtr guiToolSelector_;

        // Does player wants to quit the current level?
        bool quitLevel_;

    private:
        const unsigned int levelIndex_;

        // Variables used for sandwich reseting.
        unsigned int firstSandwich;
        unsigned int lastSandwich;

        std::mutex speedMutex;

        // Sandwiches
        Sandwich* sandwiches[N_SANDWICHES];

        m2g::TextRenderer textRenderer;

        const SDL_Color HEALTH_FONT_COLOR = { 131, 60, 60, 255 };

        // Background sprites.
        m2g::DrawablesSet backgroundSprites;
        m2g::SpritePtr grinderFront;

        SoundManager& soundManager_;

        unsigned int score_;

        unsigned int healthFontIndex_;
        unsigned int timerFontIndex_;
        unsigned int scoreFontIndex_;
};

} // namespace jdb

#endif // LEVEL_HPP
