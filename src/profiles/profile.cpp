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
    nextCampaignLevel_(0),
    survivalRecordScore_(0)
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
    return nextCampaignLevel_;
}


unsigned int Profile::survivalRecordScore() const
{
    return survivalRecordScore_;
}


/***
 * Setters
 ***/

void Profile::setNextCampaignLevel(unsigned int nextCampaignLevel)
{
    nextCampaignLevel_ = nextCampaignLevel;
}


bool Profile::updateSurvivalRecordScore(unsigned int newScore)
{
    if(newScore > survivalRecordScore_){
        survivalRecordScore_ = newScore;
        return true;
    }else{
        return false;
    }
}

} // namespace jdb
