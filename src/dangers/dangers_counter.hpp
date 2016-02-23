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

#ifndef DANGERS_FACTORY_HPP
#define DANGERS_FACTORY_HPP

#include <dangers/danger_data.hpp>
#include <map>

namespace jdb {

class DangersCounter
{
    public:
        /***
         * Construction
         ***/
        DangersCounter(unsigned int nDangers,
                       std::map<std::string, float> dangersRatios);


        /***
         * Getters
         ***/
        unsigned int nDangers() const;
        unsigned int nDangers(const std::string& dangerID) const;


    private:
        /***
         * Attributes
         ***/
        const unsigned int nDangers_;
        std::map<std::string, unsigned int> nSpecificDangers_;
};

} // namespace jdb

#endif
