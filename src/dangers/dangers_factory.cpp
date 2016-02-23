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

#include <dangers/dangers_factory.hpp>
#include <stdexcept>

namespace jdb {


/***
 * Construction
 ***/

DangersFactory::DangersFactory(
        unsigned int nDangers,
        std::vector<jdb::DangerData>& dangersData ) :
    nDangers_(nDangers)
{
    if(nDangers == 0){
        throw std::out_of_range("Can't create a dangers factory with 0 dangers!");
    }
}


/***
 * Getters
 ***/

unsigned int DangersFactory::nDangers() const
{
    return nDangers_;
}


} // namespace jdb
