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

#ifndef SANDWICHES_MANAGER
#define SANDWICHES_MANAGER

const float SANDWICHES_END_POINT = 0.0f;
const float DISTANCE_BETWEEN_SANDWICHES = 300.0f;

#include <sandwiches/sandwich.hpp>
#include <levels/conveyor_belt.hpp>
#include <vector>
#include <SFML/Graphics/Drawable.hpp>

namespace jdb {

class SandwichesManager : public sf::Drawable
{
    public:
        /***
         * Construction
         ***/
        SandwichesManager(std::vector<SandwichDataPtr> sandwichData,
                          std::unique_ptr<std::vector<DangerDataPtr>> dangerData,
                          std::unique_ptr<DangersCounter> dangersCounter,
                          std::unique_ptr<m2g::GraphicsLibrary> dangersGraphicsLibrary,
                          const ConveyorBelt& conveyorBelt);


        /***
         * Resetting
         ***/
        void reset();


        /***
         * Getters
         ***/
        unsigned int nDangersRemoved() const;
        unsigned int nInitialDangers() const;
        SandwichesVector& sandwiches();



        /***
         * Updating
         ***/
        void update(int ms, int& jacobHp, unsigned int& levelScore, float difficultyFactor);


        /***
         * Public attributes
         ***/
        std::unique_ptr< m2g::GraphicsLibrary > dangerGraphicsLibrary_;


    protected:
        /***
         * Drawable interface
         ***/
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    private:
        /***
         * Private attributes
         ***/
        // Sandwiches management
        SandwichesVector sandwiches_;
        std::vector<SandwichDataPtr> sandwichData_;

        // Variables used for sandwich reseting.
        unsigned int firstSandwich;
        unsigned int lastSandwich;

        // Dangers management.
        std::unique_ptr<std::vector<DangerDataPtr>> dangerData_;
        std::unique_ptr<DangersCounter> dangersCounter_;
        unsigned int nDangersRemoved_;

        // Used for getting the movement speed of the sandwiches.
        const ConveyorBelt& conveyorBelt_;
};

} // namespace jdb


#endif // SANDWICHES_MANAGER
