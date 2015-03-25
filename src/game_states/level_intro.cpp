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

#include "level_intro.hpp"
#include <SFML/Window/Event.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

LevelIntro::LevelIntro( const GameState& parentGameState,
                        sf::RenderWindow& window,
                        unsigned int levelIndex ) :
    GameState( window ),
    levelIndex_( levelIndex ),
    parentGameState_( parentGameState )
{}


/***
 * 2. GameState interface
 ***/

void LevelIntro::init()
{
    levelIntroFont_.loadFromFile( "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf" );
    levelIntroText_.setFont( levelIntroFont_ );
    levelIntroText_.setColor( sf::Color::Black );

    char text[250];
    sprintf( text, "Level %u\nPress any key to start", levelIndex_ );
    levelIntroText_.setString( text );

    levelIntroText_.setPosition(
                ( ( window_.getSize().x - levelIntroText_.getGlobalBounds().width ) / 2 ) +
                ( ( window_.getSize().x - levelIntroText_.getGlobalBounds().width ) / 4 ),
                ( window_.getSize().x - levelIntroText_.getGlobalBounds().width ) / 2 );
}


void LevelIntro::handleEvents()
{
    sf::Event event;

    while( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            exit( 0 );
        }else if( event.type == sf::Event::KeyPressed ){
            requestStateExit();
        }
    }
}

void LevelIntro::update( unsigned int ms )
{
    (void)(ms);
    handleEvents();
}


void LevelIntro::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
    target.draw( parentGameState_, states );
    target.draw( levelIntroText_, states );
}


void LevelIntro::pause()
{

}


void LevelIntro::resume()
{

}


} // namespace jdb
