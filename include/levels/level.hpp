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
#include <sandwiches/sandwiches_manager.hpp>

namespace jdb {

const int MAX_JACOB_LIFE = 130;

class Level : public GameState
{
    public:
        /***
         * Construction and destruction
         ***/
        Level( sf::RenderWindow& window, SoundManager* soundManager, unsigned int levelIndex, Profile& playerProfile );
        virtual ~Level() = default;


        /***
         * Getters
         ***/
        unsigned int levelIndex() const;
        unsigned int score() const;
        virtual bool defeat() const = 0;
        virtual bool victory() const = 0;
        unsigned int toolIndex() const;
        int jacobHp() const;
        float difficultyFactor() const;


        /***
         * Drawable interface
         ***/
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;


        /***
         * Player profile management (public)
         ***/
        static void savePlayerProfile(Profile& playerProfile);


    protected:
        /***
         * Loading
         ***/
        virtual bool load( unsigned int levelIndex );
        virtual tinyxml2::XMLElement* getLevelXmlNode(tinyxml2::XMLDocument& xmlFile, unsigned int index) const = 0;
        virtual std::unique_ptr<LevelIntro> generateLevelIntro(const std::vector<std::string>& newDangersIDs, tinyxml2::XMLElement* levelIntroXmlNode) const = 0;
        std::vector<SandwichDataPtr> loadSandwichData();
        void loadDangerData(tinyxml2::XMLElement* dangersXmlNode,
                            std::map<std::string, float>& dangersRatios,
                            std::vector<std::string>& newDangersIDs,
                            std::vector<DangerDataPtr>& dangerData,
                            const m2g::GraphicsLibrary& dangersGraphicsLibrary);


        /***
         * Main loop
         ***/
        void handleUserInput(const sf::Event& event, SandwichesVector& sandwiches );
        virtual void reset();


        /***
         * Inherited initializations
         ***/
        virtual std::unique_ptr<LevelUI> generateLevelUI(m2g::GraphicsLibrary& guiGraphicsLibrary) const = 0;


        /***
         * GameState interface
         ***/
        virtual void init();
        virtual void handleEvents();
        virtual void update( unsigned int ms );
        virtual void pause();
        virtual void resume();
        virtual void cleanUp();


    private:
        /***
         * Private initializations
         ***/
        void initGUI();


        /***
         * Player profile management (private)
         ***/
        virtual bool updatePlayerProfile(Profile& playerProfile) const = 0;
        void updateAndSavePlayerProfile(Profile &playerProfile) const;


    protected:
        /***
         * Attributes
         ***/
        ConveyorBelt conveyorBelt_;

        std::unique_ptr<SandwichesManager> sandwichesManager_;

        std::unique_ptr< LevelIntro > levelIntro_;

        unsigned int levelScore_;

    private:
        // Player's tool
        ToolPtr tool_;

        // Jacob's life
        int jacobHp_;

        // Level UI
        std::unique_ptr<LevelUI> levelUI_;

        SoundManager& soundManager_;

        unsigned int levelIndex_;

        Profile& playerProfile_;

        // Background sprites.
        std::vector< m2g::TileSpritePtr > backgroundSprites;
        m2g::TileSpritePtr grinderFront;

        float difficultyFactor_;
};

} // namespace jdb

#endif // LEVEL_HPP
