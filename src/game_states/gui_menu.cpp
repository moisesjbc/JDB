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

#include <game_states/gui_menu.hpp>

namespace jdb {

/***
 * Construction
 ***/

GUIMenu::GUIMenu(sf::RenderWindow &window, bool clearScreenWhenDrawing) :
    GameState(window),
    gui_(window),
    clearScreenWhenDrawing_(clearScreenWhenDrawing)
{}


/***
 * GameState interface
 ***/

void GUIMenu::init()
{
    initGUI(gui_);
}


void GUIMenu::handleEvents()
{
    sf::Event event;

    if( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            requestGameExit();
        }else if( event.type == sf::Event::KeyPressed ){
            if( event.key.code == sf::Keyboard::Escape ){
                requestStateExit();
            }
        }else{
            gui_.handleEvent( event );
        }
    }
}


void GUIMenu::update( unsigned int ms )
{
    (void)( ms );
}


void GUIMenu::pause()
{

}


void GUIMenu::resume()
{
    window_.setMouseCursorVisible( true );
}


/***
 * Drawable interface
 ***/

void GUIMenu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    (void)(target);
    (void)(states);
    if(clearScreenWhenDrawing_){
        target.clear(sf::Color( 0xDC, 0xF1, 0xF1, 0xFF ));
    }
    gui_.draw();
}

} // namespace jdb
