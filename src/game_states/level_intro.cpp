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
    parentGameState_( parentGameState ),
    gui_( window )
{}


/***
 * 2. GameState interface
 ***/

void LevelIntro::init()
{
    window_.setMouseCursorVisible( true );
    levelIntroFont_.loadFromFile( "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf" );

    char text[250];
    sprintf( text, "Level %u", levelIndex_ );

    tgui::MessageBox::Ptr messageBox( gui_ );
    messageBox->load( "data/config/gui.conf" );
    messageBox->setBackgroundColor( sf::Color::White );
    messageBox->setGlobalFont( levelIntroFont_ );
    messageBox->setText( text );
    messageBox->setTitle( "Level intro" );
    messageBox->addButton( "Start level" );
    messageBox->setSize( 400, 300 );

    messageBox->setPosition(
                (window_.getSize().x - messageBox->getSize().x) / 2,
                (window_.getSize().y - messageBox->getSize().y) / 2 );

    std::function<void(void)> messageBoxCallback =
            std::bind( &LevelIntro::requestStateExit, this, 0 );
    messageBox->bindCallback( messageBoxCallback, tgui::MessageBox:: ButtonClicked );
}


void LevelIntro::handleEvents()
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

void LevelIntro::update( unsigned int ms )
{
    (void)(ms);
    handleEvents();
}


void LevelIntro::draw( sf::RenderTarget &target, sf::RenderStates states ) const
{
    target.draw( parentGameState_, states );
    gui_.draw();
}


void LevelIntro::pause()
{

}


void LevelIntro::resume()
{

}


} // namespace jdb
