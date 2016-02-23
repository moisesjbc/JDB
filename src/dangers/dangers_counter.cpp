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
                               std::set<std::string> dangersIDs) :
    nDangers_(nDangers)
{
    if(nDangers == 0){
        throw std::out_of_range("Can't create a dangers counter with 0 dangers!");
    }

    if(dangersIDs.size() == 0){
        throw std::out_of_range("Empty dangers IDs set given to DangersCounter");
    }

    if(dangersIDs.size() > nDangers){
        throw std::runtime_error("Can't create a DangersCounter with more specific dangers than total sum of dangers");
    }

    for(const std::string& dangerID : dangersIDs){
        nSpecificDangers_[dangerID] = nDangers / dangersIDs.size();
    }
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


} // namespace jdb
