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

#ifndef DANGER_HPP
#define DANGER_HPP

#include "player_action.hpp"
#include "danger_data.hpp"
#include <m2g/drawables/animation.hpp>
#include <dangers/danger_mutation.hpp>

namespace jdb {

// TODO: Move back to game's main loop and pass the FPS_INVERSE to the dangers
// in other way.
const unsigned int FPS = 25;
const unsigned int REFRESH_TIME = 1000 / FPS;
const float FPS_INVERSE = 1.0f / FPS;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

class Danger : public m2g::Animation
{
    private:
        float hp;
        unsigned int state;

        DangerDataPtr dangerData;

        // This is an extra animation displayed over the danger when this
        // firstly appears.
        std::unique_ptr< m2g::Animation > appearanceAnimation;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Danger( DangerDataPtr dangerData_,
                m2g::AnimationData* appearanceAnimationData = nullptr );
        virtual ~Danger() = default;


        /***
         * 2. Getters
         ***/
        float getDamage() const ;
        DangerDataPtr getDangerData() const ;
        bool onScreen() const;


        /***
         * 3. Setters
         ***/
        // TODO: Overload Animation setters.
        void setDangerData( DangerDataPtr dangerData_,
                            m2g::AnimationData* appearanceAnimationData = nullptr );
        void setDangerDataWithUniqueAnimationData( DangerDataPtr dangerData_,
                                                   m2g::AnimationDataPtr appearanceAnimationData = nullptr );
        void setState( int newState );


        /***
         * 4. Updating
         ***/
        virtual void update(unsigned int ms,
                            int& playerHp,
                            unsigned int& playerScore,
                            m2g::GraphicsLibrary& dangersGraphicsLibrary,
                            float difficultyFactor);
        bool playerAction( PlayerAction playerAction,
                           int& playerHp,
                           unsigned int& playerScore,
                           m2g::GraphicsLibrary& dangersGraphicsLibrary,
                           float difficultyFactor );
        void applyMutation(const DangerMutation& mutation,
                           int& playerHp,
                           unsigned int& playerScore,
                           m2g::GraphicsLibrary& dangersGraphicsLibrary,
                           float difficultyFactor );
        void reset();
        StunType stuns( const m2g::TileSprite &tool, ToolType toolType ) const;


        /***
         * 5. Drawing
         ***/
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

} // namespace jdb

#endif // DANGER_HPP
