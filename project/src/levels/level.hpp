/***
    Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)

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

#include <stdexcept>
#include "../dependencies/m2g/src/graphics_library.hpp"
#include "../dependencies/m2g/src/drawables/drawables_set.hpp"
#include <glm/gtx/matrix_operation.hpp>
#include <SDL2/SDL_video.h>
#include <thread>
#include <mutex>
#include "../utilities/timer.hpp"
#include <functional>
#include "../dangers/danger.hpp"
#include "../tools/tool.hpp"
#include "../sandwiches/sandwich.hpp"
#include "../dependencies/m2g/src/text/text_renderer.hpp"
#include "../dependencies/m2g/src/drawables/drawables_set.hpp"
#include <freetype2/freetype/config/ftheader.h>
#include <FTGL/ftgl.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include "conveyor_belt.hpp"

typedef std::function< bool () > FinishPredicate;

namespace jdb {

enum class LevelType
{
    CAMPAIGN,
    SURVIVAL
};


class Level
{
    protected:
        ConveyorBelt conveyorBelt_;

        tinyxml2::XMLDocument xmlFile;

        m2g::GraphicsLibrary graphicsLibrary_;

        std::vector< DangerDataPtr > dangerData;
        std::vector< SandwichDataPtr > sandwichData;

        SDL_Window* window;
        SDL_Surface* screen;
        glm::mat4 projectionMatrix;

        std::mutex coutMutex;

        // Players tool
        ToolPtr tool_;

        // Timer
        Timer timer;

        // Jacob's life
        int jacobHp_;

        // GUI sprites.
        m2g::DrawablesSet guiSprites_;
        m2g::SpritePtr guiToolSelector_;

        // Does player wants to quit the current level?
        bool quitLevel_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Level( SDL_Window* window_, SDL_Surface* screen_, unsigned int screenWidth, unsigned int screenHeight );

    private:
        void initGUI();


        /***
         * 2. Level Loading
         ***/
    protected:
        virtual void load( unsigned int levelIndex ) = 0;
        void loadSandwichData();
        void loadDangerData();


        /***
         * 3. Level execution
         ***/
    public:
        void run( unsigned int levelIndex );


        /***
         * 4. Main loop
         ***/
    protected:
        void mainLoop();
        void handleUserInput( const SDL_Event& event, Sandwich** sandwiches );
        virtual bool finishPredicate() const = 0;


        /***
         * 5. Auxiliar methods
         ***/
    private:
        void drawTimer( int time );
};

} // namespace jdb

#endif // LEVEL_HPP
