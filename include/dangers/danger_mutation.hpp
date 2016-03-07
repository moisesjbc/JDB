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

#ifndef DANGER_MUTATION_HPP
#define DANGER_MUTATION_HPP

#include <string>

namespace jdb {

const int HP_ALL = 156474;

typedef std::string DangerID;
const DangerID DANGER_NULL_ID = "";

class DangerMutation
{
    public:
        /***
         * Construction
         ***/
        DangerMutation(int dangerHpVariation = 0,
                       int newDangerState = -1,
                       DangerID newDanger = DANGER_NULL_ID,
                       std::string newDangerAppearanceAnimation = "",
                       int playerHpVariation = 0,
                       int playerScoreVariation = 0);


        /***
         * Getters
         ***/
        int dangerHpVariation() const;
        int newDangerState() const;
        DangerID newDanger() const;
        std::string newDangerAppearanceAnimaton() const;
        int playerHpVariation() const;
        int playerScoreVariation() const;


    private:
        /***
         * Attributes
         ***/
        const int dangerHpVariation_;
        const int newDangerState_;
        const DangerID newDanger_;
        std::string newDangerAppearanceAnimation_;
        const int playerHpVariation_;
        const int playerScoreVariation_;
};


} // namespace jdb

#endif
