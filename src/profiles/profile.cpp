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


#include <profiles/profile.hpp>

namespace jdb {

/***
 * Construction
 ***/

Profile::Profile(const std::string& name) :
    name_(name),
    survivalRecordScore_(0),
    gameDifficulty_(GameDifficulty::NORMAL)
{
    if(name.length() < 3){
        throw std::invalid_argument("Profile name with less than 3 characters not allowed");
    }
}


/***
 * Getters
 ***/

std::string Profile::name() const
{
    return name_;
}


unsigned int Profile::nextCampaignLevel() const
{
    return campaignRecordScores_.size();
}


unsigned int Profile::survivalRecordScore() const
{
    return survivalRecordScore_;
}


unsigned int Profile::campaignLevelRecordScore(unsigned int levelIndex) const
{
    if(levelIndex < campaignRecordScores_.size()){
        return campaignRecordScores_.at(levelIndex);
    }else{
        return 0;
    }
}


GameDifficulty Profile::gameDifficulty() const
{
    return gameDifficulty_;
}


/***
 * Setters
 ***/


bool Profile::updateSurvivalRecordScore(unsigned int newScore)
{
    if(newScore > survivalRecordScore_){
        survivalRecordScore_ = newScore;
        return true;
    }else{
        return false;
    }
}


bool Profile::updateCampaignLevelRecordScore(unsigned int levelIndex, unsigned int newScore)
{
    if(levelIndex < campaignRecordScores_.size()){
        if(newScore > campaignRecordScores_[levelIndex]){
            campaignRecordScores_[levelIndex] = newScore;
            return true;
        }else{
            return false;
        }
    }else{
        campaignRecordScores_.resize(levelIndex + 1, 0);
        campaignRecordScores_[levelIndex] = newScore;
        return true;
    }
}


bool Profile::updateGameDifficulty(GameDifficulty newGameDifficulty)
{
    bool gameDifficultyChanged = (newGameDifficulty != gameDifficulty_);
    gameDifficulty_ = newGameDifficulty;
    return gameDifficultyChanged;
}

} // namespace jdb
