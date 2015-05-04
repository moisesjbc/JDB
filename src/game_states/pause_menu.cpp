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
#include <TGUI/Panel.hpp>
#include <TGUI/Button.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

PauseMenu::PauseMenu( sf::RenderWindow& window, const GameState& parentGameState ) :
    GameState( window ),
    parentGameState_( parentGameState ),
    gui_( window )
{}


/***
 * 3. GameState interface
 ***/

void PauseMenu::init()
{
    gui_.setGlobalFont( "data/fonts/LiberationSans-Bold.ttf" );

    tgui::Panel::Ptr pausePanel = tgui::Panel::create( { 250, 200 });
    pausePanel->setBackgroundColor( sf::Color::White );
    pausePanel->setPosition({
                                ( window_.getSize().x - pausePanel->getSize().x ) / 2.0f,
                                ( window_.getSize().y - pausePanel->getSize().y ) / 2.0f
                            });

    tgui::Label::Ptr pauseMenuLabel = tgui::Label::create();
    pauseMenuLabel->setText( "Game paused" );
    pauseMenuLabel->setTextColor( sf::Color::Black );
    pauseMenuLabel->setPosition( 0.0f, 0.0f );

    tgui::Button::Ptr resumeGameButton = tgui::Button::create();
    resumeGameButton->setText( "Resume game" );
    resumeGameButton->setTextSize( 14 );
    resumeGameButton->setPosition({
                                   ( pausePanel->getSize().x - resumeGameButton->getSize().x ) / 2.0f,
                                   ( pausePanel->getSize().y - resumeGameButton->getSize().y ) / 2.0f
                                   });
    resumeGameButton->connect( "pressed", [this](){ requestStateExit(); } );

    tgui::Button::Ptr returnToMainMenuButton = tgui::Button::create();
    returnToMainMenuButton->setText( "Return to main menu" );
    returnToMainMenuButton->setTextSize( 14 );
    returnToMainMenuButton->setPosition({
                                   ( pausePanel->getSize().x - resumeGameButton->getSize().x ) / 2.0f,
                                   tgui::bindBottom( resumeGameButton ) + 10.0f
                                   });
    returnToMainMenuButton->connect( "pressed", [this](){ requestStateExit( RETURN_TO_MAIN_MENU_REQUESTED ); } );

    pausePanel->add( pauseMenuLabel );
    pausePanel->add( resumeGameButton );
    pausePanel->add( returnToMainMenuButton );

    gui_.add( pausePanel );

    window_.setMouseCursorVisible( true );
}


void PauseMenu::handleEvents()
{
    sf::Event event;

    while( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            exit( 0 );
        }else{
            gui_.handleEvent( event );
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
    gui_.draw();
}


void PauseMenu::pause()
{

}


void PauseMenu::resume()
{
    window_.setMouseCursorVisible( true );
}


} // namespace jdb
