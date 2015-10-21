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

#include "jdb.hpp"
#include <utilities/sound_manager.hpp>
#include <boost/filesystem.hpp>

int main( int argc, char* argv[] )
{
    boost::filesystem::current_path( boost::filesystem::path( argv[0] ).parent_path() );

    // Make sure the random numbers generator gets a new seed each time the
    // game is run.
    srand( time( nullptr ) );

    jdb::JDB jdb;
    jdb.run();

    return 0;
}

