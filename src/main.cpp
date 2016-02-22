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

#include "jdb.hpp"
#include <utilities/sound_manager.hpp>
#include <boost/filesystem.hpp>
#include <paths.hpp>


void InitializeLoggingSystem(int argc, char* argv[])
{
    const std::string LOG_FILE_PATH = jdb::DATA_DIR_PATH + "/sandwiches_game.log";

    START_EASYLOGGINGPP(argc, argv);

    // Create logging file.
    std::ofstream file(LOG_FILE_PATH);
    file.close();

    // Make eassyloggingpp log to file.
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.set(el::Level::Global, el::ConfigurationType::Filename, LOG_FILE_PATH);
    el::Loggers::reconfigureLogger("default", defaultConf);

    LOG(INFO) << "Initializing The Sandwiches Game!";
}

int main( int argc, char* argv[] )
{
    InitializeLoggingSystem(argc, argv);

    boost::filesystem::current_path( boost::filesystem::path( argv[0] ).parent_path() );

    // Make sure the random numbers generator gets a new seed each time the
    // game is run.
    srand( time( nullptr ) );

    jdb::JDB jdb;
    jdb.run();

    return 0;
}

