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

#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <iostream>
#include <stdexcept>
#include <vector>
#include <utilities/game_difficulty.hpp>

namespace jdb {

class Profile
{
    public:
        /***
         * Construction
         ***/
        Profile(const std::string& name);


        /***
         * Getters
         ***/
        std::string name() const;
        unsigned int nextCampaignLevel() const;
        unsigned int survivalRecordScore() const;
        unsigned int campaignLevelRecordScore(unsigned int levelIndex) const;
        GameDifficulty gameDifficulty() const;


        /***
         * Setters
         ***/
        bool updateSurvivalRecordScore(unsigned int newScore);
        bool updateCampaignLevelRecordScore(unsigned int levelIndex, unsigned int newScore);
        bool updateGameDifficulty(GameDifficulty newGameDifficulty);


    private:
        std::string name_;
        unsigned int survivalRecordScore_;
        std::vector<unsigned int> campaignRecordScores_;
        GameDifficulty gameDifficulty_;
};

} // namespace jdb

#endif // PROFILE_HPP
