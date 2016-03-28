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

#ifndef SANDWICH_HPP
#define SANDWICH_HPP

#include "sandwich_data.hpp"
#include "../dangers/danger.hpp"
#include <array>
#include <vector>
#include <dangers/dangers_counter.hpp>

namespace jdb {

const unsigned int MAX_DANGERS_PER_SANDWICH = 3;

class Sandwich : public m2g::Animation
{
    private:
        SandwichDataPtr sandwichData_;

        unsigned int nDangers_;
        std::array< Danger*, MAX_DANGERS_PER_SANDWICH > dangers_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Sandwich( SandwichDataPtr sandwichData,
                  const std::vector< DangerDataPtr >* dangerData );
        virtual ~Sandwich();

        /***
         * 2. Loading
         ***/
        void setSandwichData( SandwichDataPtr sandwichData );
        void populate(const std::vector< DangerDataPtr >& dangerData,
                      DangersCounter* dangersCounter = nullptr);


        /***
         * 3. Getters
         ***/
        float getDamage() const;
        std::vector<std::string> getDangersIDs() const;
        unsigned int nDangers() const;


        /***
         * 4. Transformations
         ***/
        virtual void translate( const float& tx, const float& ty );
        virtual void setPosition( const float& x, const float& y );


        /***
         * 5. Updating
         ***/
        virtual void update( unsigned int ms,
                             int& playerHp,
                             unsigned int& playerScore,
                             m2g::GraphicsLibrary& dangersGraphicsLibrary,
                             float difficultyFactor);
        bool useTool( PlayerAction playerAction,
                      TileSprite* tool,
                      int& playerHp,
                      unsigned int& playerScore,
                      m2g::GraphicsLibrary& dangersGraphicsLibrary,
                      float difficultyFactor);
        StunType stuns( const m2g::TileSprite &tool, ToolType toolType );
        void reset();


        /***
         * 6. Drawing
         ***/
        virtual void draw( sf::RenderTarget &target, sf::RenderStates states ) const;
};

typedef std::vector< std::unique_ptr< Sandwich > > SandwichesVector;

} // namespace jdb

#endif // SANDWICH_HPP
