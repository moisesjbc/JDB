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

#include <dangers/danger_mutation.hpp>

namespace jdb {

/***
 * Construction
 ***/

DangerMutation::DangerMutation(int dangerHpVariation,
                               int newDangerState,
                               DangerID newDanger,
                               std::string newDangerAppearanceAnimation,
                               int playerHpVariation,
                               int playerScoreVariation) :
    dangerHpVariation_(dangerHpVariation),
    newDangerState_(newDangerState),
    newDanger_(newDanger),
    newDangerAppearanceAnimation_(newDangerAppearanceAnimation),
    playerHpVariation_(playerHpVariation),
    playerScoreVariation_(playerScoreVariation)
{}


/***
 * Getters
 ***/

int DangerMutation::dangerHpVariation() const
{
    return dangerHpVariation_;
}


int DangerMutation::newDangerState() const
{
    return newDangerState_;
}


DangerID DangerMutation::newDanger() const
{
    return newDanger_;
}


std::string DangerMutation::newDangerAppearanceAnimaton() const
{
    return newDangerAppearanceAnimation_;
}


int DangerMutation::playerHpVariation() const
{
    return playerHpVariation_;
}


int DangerMutation::playerScoreVariation() const
{
    return playerScoreVariation_;
}

} // namespace jdb
