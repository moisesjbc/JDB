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

#include <utilities/sound_manager.hpp>
#include <boost/filesystem.hpp>
#include <paths.hpp>

using namespace boost::filesystem;

namespace jdb {

Sound newSound;

/***
 * 1. Construction
 ***/

SoundManager::SoundManager( const char *soundsDirPath ) :
    SOUNDS_DIR_PATH( soundsDirPath )
{
    boost::filesystem::path musicPath =
            (boost::filesystem::path(DATA_DIR_PATH) / boost::filesystem::path("/audio/music/26979__raggaman__3er-ding-01.ogg"));
    if(!music_.openFromFile(musicPath.string()))
        throw std::runtime_error("Error loading music from file - " + musicPath.string());
    music_.setLoop( true );
    music_.play();
}


/***
 * 3. Sounds loading
 ***/

SoundIndex SoundManager::loadSounds( const std::string& category,
                                     const std::string& prefix )
{
    const SoundIndex soundIndex = sounds_.size();

    path dirPath( SOUNDS_DIR_PATH + "/" + category );

    if( exists( dirPath ) && is_directory( dirPath ) ){
        const directory_iterator endIterator;
        directory_iterator fileIterator( dirPath );

        for( ; fileIterator != endIterator; fileIterator++ ){
            if( is_regular_file( *fileIterator ) ){
                const std::string fileName =
                        basename( fileIterator->path() );

                if( fileName.substr( 0, prefix.size() ) == prefix ){
                    if( !newSound.buffer.loadFromFile( fileIterator->path().string() ) ){
                        throw std::runtime_error( std::string( "Error loading sound [" ) +
                                                  fileIterator->path().string() +
                                                  "]" );
                    }
                    newSound.sound.setBuffer( newSound.buffer );
                    newSound.sound.setLoop( false );

                    sounds_[soundIndex].push_back( newSound );
                }
            }
        }

        return soundIndex;
    }else{
        throw std::runtime_error( std::string( "Error loading sounds [" ) +
                                  dirPath.string() +
                                  std::string( "] doesn't exist or isn't a dir" ) );
    }
}


/***
 * 4. Playing
 ***/

void SoundManager::playSound( SoundIndex soundIndex )
{
    // Select a random sound among those sharing the same sound index.
    unsigned int finalSoundIndex = rand()%sounds_.at( soundIndex ).size();

    // Play the selected sound.
    sf::Sound& sound = sounds_.at( soundIndex ).at( finalSoundIndex ).sound;
    sound.setVolume( sf::Listener::getGlobalVolume() );
    sound.play();
}

} // namespace jdb
