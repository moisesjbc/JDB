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

#ifndef SOUND_MANAGER_HPP
#define SOUND_MANAGER_HPP

#include <string>
#include <SFML/Audio.hpp>
#include <map>
#include <vector>

namespace jdb {

typedef unsigned int SoundIndex;

struct Sound {
    sf::SoundBuffer buffer;
    sf::Sound sound;
};

class SoundManager
{
    public:
        /***
         * 1. Construction
         ***/
        SoundManager( const char* soundsDirPath );


        /***
         * 2. Destruction
         ***/
        ~SoundManager() = default;


        /***
         * 3. Sounds loading
         ***/

        // Load all the sounds whose paths are in the form
        // SOUNDS_DIR_PATH/category/prefix*
        SoundIndex loadSounds( const std::string& category,
                               const std::string& prefix );


        /***
         * 4. Playing
         ***/
        void playSound( SoundIndex soundIndex );


    private:
        const std::string SOUNDS_DIR_PATH;

        std::map< SoundIndex, std::vector< Sound > > sounds_;

        sf::Music music_;
};

} // namespace jdb

#endif // SOUND_MANAGER_HPP
