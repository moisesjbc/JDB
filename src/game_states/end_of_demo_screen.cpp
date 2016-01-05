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

#include "end_of_demo_screen.hpp"
#include <paths.hpp>
#include <boost/filesystem.hpp>
#include <utilities/unicode_file_reader.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

EndOfDemoScreen::EndOfDemoScreen( sf::RenderWindow& window, const GameState& parentGameState ) :
    GameState( window ),
    parentGameState_( parentGameState ),
    gui_( window )
{}


/***
 * 2. GameState interface
 ***/

void EndOfDemoScreen::init()
{
    gui_.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();
    layout->setSize( tgui::bindSize(gui_) * 0.90f);
    layout->setPosition( tgui::bindSize(gui_) * 0.05f );

    tgui::TextBox::Ptr textBox = std::make_shared<tgui::TextBox>();
    textBox->setReadOnly( true );
    textBox->setText( readEndOfDemoText() );

    tgui::Button::Ptr exitButton = std::make_shared<tgui::Button>();
    exitButton->setText( "Return" );
    exitButton->setTextSize( 20 );
    exitButton->connect( "pressed", [this](){ requestStateExit(); } );

    layout->add(textBox);
    layout->setRatio(textBox, 9);
    layout->insertSpace(1, 0.1f);

    layout->add(exitButton);
    layout->setRatio(exitButton, 1);

    gui_.add(layout);

    window_.setMouseCursorVisible( true );
}


void EndOfDemoScreen::handleEvents()
{
    sf::Event event;

    while( window_.pollEvent( event ) ){
        if( event.type == sf::Event::Closed ){
            requestGameExit();
        }else{
            gui_.handleEvent( event );
        }
    }
}


void EndOfDemoScreen::update( unsigned int ms )
{
    (void)( ms );
}


void EndOfDemoScreen::pause()
{

}


void EndOfDemoScreen::resume()
{
    window_.setMouseCursorVisible( true );
}


/***
 * 3. Drawable interface
 ***/

void EndOfDemoScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw( parentGameState_, states );
    gui_.draw();
}


/***
 * 4. Auxiliar initialization methods
 ***/

std::wstring EndOfDemoScreen::readEndOfDemoText()
{
    return UnicodeFileReader::readUTF8((boost::filesystem::path(DATA_DIR_PATH) / boost::filesystem::path("end_of_demo")).string());
}

} // namespace jdb
