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

#include <game_states/end_of_campaign_screen.hpp>
#include <paths.hpp>
#include <boost/filesystem.hpp>
#include <utilities/unicode_file_reader.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

namespace jdb {

/***
 * Construction
 ***/

EndOfCampaignScreen::EndOfCampaignScreen( sf::RenderWindow& window, const GameState& parentGameState ) :
    GUIMenu( window, false ),
    parentGameState_( parentGameState )
{
    boost::filesystem::path texturePath =
        (boost::filesystem::path(DATA_DIR_PATH) / boost::filesystem::path("/img/victory_screen.png"));
    if(!backgroundTexture_.loadFromFile(texturePath.string())){
        throw std::runtime_error("Couldn't load " + texturePath.string());
    }
    backgroundSprite_.setPosition(0, 0);
    backgroundSprite_.setTexture(backgroundTexture_);
}


/***
 * GameState interface
 ***/

void EndOfCampaignScreen::initGUI(tgui::Gui& gui)
{
    gui.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();
    layout->setSize( tgui::bindWidth(gui), tgui::bindHeight(gui) * 0.20f);
    layout->setPosition(sf::Vector2f(0.0f, 0.0f));

    tgui::TextBox::Ptr textBox = std::make_shared<tgui::TextBox>();
    textBox->setReadOnly( true );
    textBox->setText( readEndOfCampaignText() );

    tgui::Button::Ptr exitButton = std::make_shared<tgui::Button>();
    exitButton->setText( "Return" );
    exitButton->setTextSize( 20 );
    exitButton->connect( "pressed", [this](){ requestStateExit(); } );

    layout->add(textBox);
    layout->setRatio(textBox, 9);
    layout->insertSpace(1, 0.1f);

    layout->add(exitButton);
    layout->setRatio(exitButton, 1);

    gui.add(layout);

    window_.setMouseCursorVisible( true );
}


/***
 * Drawable interface
 ***/

void EndOfCampaignScreen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(backgroundSprite_, states);
    GUIMenu::draw(target, states);
}


/***
 * Auxiliar initialization methods
 ***/

std::wstring EndOfCampaignScreen::readEndOfCampaignText()
{
    return UnicodeFileReader::readUTF8((boost::filesystem::path(DATA_DIR_PATH) / boost::filesystem::path("end_of_campaign")).string());
}

} // namespace jdb
