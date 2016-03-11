/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

    This file is part of JDB.

    JDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JDB.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include <dangers/dangers_counter.hpp>
#include <stdexcept>

namespace jdb {


/***
 * Construction
 ***/

DangersCounter::DangersCounter(unsigned int nDangers,
                               std::map<std::string, float> dangersRatios) :
    nDangers_(nDangers),
    initialNDangers_(nDangers)
{
    if(nDangers == 0){
        throw std::out_of_range("Can't create a dangers counter with 0 dangers!");
    }

    if(dangersRatios.size() == 0){
        throw std::out_of_range("Empty dangers ratios set given to DangersCounter");
    }

    if(dangersRatios.size() > nDangers){
        throw std::runtime_error("Can't create a DangersCounter with more specific dangers than total sum of dangers");
    }

    float accumulatedRatio = 0.0f;
    for(const std::pair<std::string, float> dangerRatioPair : dangersRatios){
        accumulatedRatio += dangerRatioPair.second;
    }

    unsigned int nDistributedDangers = 0;
    for(const std::pair<std::string, float> dangerRatioPair : dangersRatios){
        nSpecificDangers_[dangerRatioPair.first] = nDangers * dangerRatioPair.second / accumulatedRatio;
        nDistributedDangers += nSpecificDangers_[dangerRatioPair.first];
    }

    // If the distribution of dangers total in specific dangers was not exact,
    // assign the reminder to the most frequent danger.
    if(nDangers > nDistributedDangers){
        std::map<std::string, unsigned int>::iterator it =
            nSpecificDangers_.begin();
        std::map<std::string, unsigned int>::iterator mostFrequentDangerIt =
            it;
        while(it != nSpecificDangers_.end()){
            if(it->second > mostFrequentDangerIt->second){
                mostFrequentDangerIt = it;
            }
            it++;
        }
        mostFrequentDangerIt->second += (nDangers - nDistributedDangers);
    }

    initialNSpecificDangers_ = nSpecificDangers_;
}


/***
 * Getters
 ***/

unsigned int DangersCounter::nDangers() const
{
    return nDangers_;
}


unsigned int DangersCounter::nDangers(const std::string& dangerID) const
{
    try{
        return nSpecificDangers_.at(dangerID);
    }catch(std::out_of_range&){
        throw std::out_of_range("Danger [" + dangerID + "] not present in dangers counter");
    }
}

unsigned int DangersCounter::initialNDangers() const
{
    return initialNDangers_;
}


/***
 * Modificators
 ***/

void DangersCounter::decreaseDangerCounter(const std::string &dangerID)
{
    if(initialNDangers_ != UNLIMITED_DANGERS){
        if(nSpecificDangers_.at(dangerID) > 0){
            nSpecificDangers_.at(dangerID)--;
            nDangers_--;
        }else{
            throw std::runtime_error("Triying to decrease a zero counter");
        }
    }
}


void DangersCounter::reset()
{
    nDangers_ = initialNDangers_;
    nSpecificDangers_ = initialNSpecificDangers_;
}


} // namespace jdb
