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

#include "main_menu.hpp"
#include "../levels/campaign_level.hpp"
#include "../levels/survival_level.hpp"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Font.hpp>

namespace jdb {

/***
 * 1. Creation
 ****/

MainMenu::MainMenu( sf::RenderWindow& window, SoundManager* soundManager ) :
    GameState( window ),
    soundManager_( *soundManager )
{}


/***
 * 3. GameState interface
 ***/

void MainMenu::init()
{
    if( !font_.loadFromFile( "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf" ) ){
        throw std::runtime_error( "Couldn't load font" );
    }

    menuText_.setFont( font_ );
    menuText_.setColor( sf::Color::Black );
    menuText_.setString( "C - Play campaign\nS - Play survival\nESC - Exit" );
}


void MainMenu::handleEvents()
{
    sf::Event event;
    std::unique_ptr< Level > level = nullptr;

    if( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            requestStateExit();
        }else if( event.type == sf::Event::KeyPressed ){
            if( event.key.code == sf::Keyboard::Escape ){
                requestStateExit();
            }else if( event.key.code == sf::Keyboard::C ){
                std::unique_ptr< Level > level = std::unique_ptr< Level >(
                           new SurvivalLevel( window_, &soundManager_, 0 ) );
                switchState( *level );
            }else if( event.key.code == sf::Keyboard::S ){
                std::unique_ptr< Level > level = std::unique_ptr< Level >(
                           new SurvivalLevel( window_, &soundManager_, 0 ) );
                switchState( *level );
            }
        }
    }
}


void MainMenu::update( unsigned int ms )
{
    (void)( ms );
}


void MainMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    (void)( target );
    (void)( states );
    window_.clear( sf::Color( 0xDC, 0xF1, 0xF1, 0xFF ) );

    window_.draw( menuText_ );
}


void MainMenu::pause()
{
}

void MainMenu::resume()
{
    SDL_ShowCursor( SDL_ENABLE );
}

} // namespace jdb
