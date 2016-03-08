/***
    Copyright 2013 - 2016 Moises J. Bonilla Caraballo (moisesjbc)

    This file is part of sandwiches-game.

    sandwiches-game is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sandwiches-game is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sandwiches-game.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <SFML/Window/Event.hpp>
#include "../sandwiches/sandwich.hpp"
#include "../dangers/danger.hpp"
#include "../tools/tool.hpp"
#include "conveyor_belt.hpp"
#include <game_states/game_state.hpp>
#include <game_states/pause_menu.hpp>
#include <game_states/level_intro.hpp>
#include <paths.hpp>
#include <utilities/easylogging++.h>
#include <dangers/dangers_counter.hpp>
#include <profiles/profile.hpp>
#include <level_ui/level_ui.hpp>

namespace jdb {

const unsigned int N_SANDWICHES = 4;

class Level : public GameState
{
    public:
        /***
         * 1. Construction
         ***/
        Level( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex, Profile& playerProfile );


        /***
         * 2. Destruction
         ***/
        virtual ~Level() = default;


        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    protected:
        /***
         * 3. Level Loading
         ***/
        virtual bool load( unsigned int levelIndex ) = 0;
        void loadSandwichData();
        void loadDangerData(tinyxml2::XMLElement *dangersXmlNode,
                            std::map<std::string, float>& dangersRatios,
                            std::vector<std::string>& newDangersIDs);


        /***
         * 4. Main loop
         ***/
        void handleUserInput(const sf::Event& event, SandwichesVector& sandwiches );
        virtual bool defeat() const = 0;
        virtual bool victory() const = 0;
        void reset();


        /***
         * 5. Auxiliar methods
         ***/
        int getSeconds() const ;
        unsigned int levelIndex() const;
        unsigned int nSandwiches() const;
        unsigned int score() const;

        virtual void resetLevelTime() = 0;
        virtual void updateLevelTime( unsigned int ms ) = 0;
        virtual void drawLevelProgress() const = 0;
        virtual std::unique_ptr<LevelUI> generateLevelUI(m2g::GraphicsLibrary& guiGraphicsLibrary) const = 0;


        /***
         * 6. GameState interface
         ***/
        virtual void init();
        virtual void handleEvents();
        virtual void update( unsigned int ms );
        virtual void pause();
        virtual void resume();
        virtual void cleanUp();


    private:
        /***
         * 7. Initialization
         ***/
        void initGUI();


        /***
         * Player profile management
         ***/
        virtual bool updatePlayerProfile(Profile& playerProfile) const = 0;
        void savePlayerProfile(Profile& playerProfile) const;
        void updateAndSavePlayerProfile(Profile &playerProfile) const;


    protected:
        /***
         * Attributes
         ***/
        ConveyorBelt conveyorBelt_;

        std::vector< DangerDataPtr > dangerData;
        std::vector< SandwichDataPtr > sandwichData;

        // Players tool
        ToolPtr tool_;

        // Jacob's life
        int jacobHp_;

        std::unique_ptr<LevelUI> levelUI_;

        SoundManager& soundManager_;

        unsigned int levelTime_;

        std::unique_ptr< LevelIntro > levelIntro_;
        unsigned int levelIndex_;

        std::unique_ptr<DangersCounter> dangersCounter_;
        mutable sf::Text progressText_;

        unsigned int nDangersRemoved_ = 0;

        Profile& playerProfile_;

        unsigned int acumScore_;
        unsigned int levelScore_;

    private:
        // Variables used for sandwich reseting.
        unsigned int firstSandwich;
        unsigned int lastSandwich;

        // Sandwiches
        SandwichesVector sandwiches;

        // Background sprites.
        std::vector< m2g::TileSpritePtr > backgroundSprites;
        m2g::TileSpritePtr grinderFront;

        std::unique_ptr< m2g::GraphicsLibrary > dangerGraphicsLibrary_;
};

} // namespace jdb

#endif // LEVEL_HPP
