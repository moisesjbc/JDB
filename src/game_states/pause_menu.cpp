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

#include "pause_menu.hpp"
#include <SFML/Window/Event.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

PauseMenu::PauseMenu( sf::RenderWindow& window, const GameState& parentGameState ) :
    GameState( window ),
    parentGameState_( parentGameState )
{}


/***
 * 3. GameState interface
 ***/

void PauseMenu::init()
{
    pauseMenuFont_.loadFromFile( "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf" );

    pauseMenuText_.setFont( pauseMenuFont_ );
    pauseMenuText_.setCharacterSize( 30 );
    pauseMenuText_.setColor( sf::Color::Black );
    pauseMenuText_.setString( "Press any key to resume game" );

    pauseMenuText_.setPosition( ( window_.getSize().x - pauseMenuText_.getLocalBounds().width ) / 2,
                                ( window_.getSize().y - pauseMenuText_.getLocalBounds().height ) / 2 );

    window_.setMouseCursorVisible( true );
}


void PauseMenu::handleEvents()
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


void PauseMenu::update( unsigned int ms )
{
    (void)( ms );
}


void PauseMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw( parentGameState_, states );
    target.draw( pauseMenuText_, states );
}


void PauseMenu::pause()
{

}


void PauseMenu::resume()
{
    window_.setMouseCursorVisible( true );
}


} // namespace jdb
