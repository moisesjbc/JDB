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

#include <game_states/credits_screen.hpp>
#include <boost/filesystem.hpp>

namespace jdb {

/***
 * 1. Construction
 ***/

CreditsScreen::CreditsScreen( sf::RenderWindow& window ) :
    GUIMenu( window )
{}


/***
 * 2. GameState interface
 ***/

void CreditsScreen::initGUI(tgui::Gui &gui)
{
    gui.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();
    layout->setSize( tgui::bindSize(gui) * 0.90f);
    layout->setPosition( tgui::bindSize(gui) * 0.05f );

    tgui::TextBox::Ptr textBox = std::make_shared<tgui::TextBox>();
    textBox->setReadOnly( true );
    textBox->setText( generateCredits() );

    tgui::Button::Ptr exitButton = std::make_shared<tgui::Button>();
    exitButton->setText( "Exit credits" );
    exitButton->setTextSize( 20 );
    exitButton->connect( "pressed", [this](){ requestStateExit(); } );

    layout->add(textBox);
    layout->setRatio(textBox, 9);
    layout->insertSpace(1, 0.1f);

    layout->add(exitButton);
    layout->setRatio(exitButton, 1);

    gui.add(layout);
}


/***
 * 4. Credits generation
 ***/

std::wstring CreditsScreen::generateCredits() const
{
    std::wstring credits;

    const std::vector<std::string> filepaths =
    {
        "/LICENSE",
        "/img/LICENSE",
        "/audio/music/LICENSE",
        "/audio/player/LICENSE",
        "/audio/tools/LICENSE",
        "/fonts/LICENSE"
    };

    for( const std::string& filepath : filepaths ){
        std::wstring fileContent =
                UnicodeFileReader::readUTF8((boost::filesystem::path(DATA_DIR_PATH) / boost::filesystem::path(filepath)).string());

        credits += fileContent + L"\n";
    }

    return credits;
}


} // namespace jdb
