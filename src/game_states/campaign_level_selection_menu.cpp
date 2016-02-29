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

#include <game_states/campaign_level_selection_menu.hpp>
#include <levels/campaign_level.hpp>

namespace jdb {


/***
 * Construction
 ***/

CampaignLevelSelectionMenu::CampaignLevelSelectionMenu(sf::RenderWindow &window,
                                                       SoundManager &soundManager,
                                                       unsigned int nLevels,
                                                       Profile& playerProfile ) :
    GUIMenu(window),
    soundManager_(soundManager),
    nLevels_(nLevels),
    playerProfile_(playerProfile)
{}


/***
 * GUIMenu interface
 ***/

void CampaignLevelSelectionMenu::initGUI(tgui::Gui &gui)
{
    gui.setFont( DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf" );

    tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();
    layout->setSize( tgui::bindSize(gui) * 0.90f);
    layout->setPosition( tgui::bindSize(gui) * 0.05f );

    for(unsigned int levelIndex=0; levelIndex<nLevels_; levelIndex++){
        tgui::Button::Ptr playLevelButton = std::make_shared<tgui::Button>();
        playLevelButton->setText( "Play level " + std::to_string(levelIndex) );
        playLevelButton->setTextSize( 20 );
        playLevelButton->connect( "pressed", [=](){
            std::unique_ptr< Level > level = std::unique_ptr< Level >(
                       new CampaignLevel( window_, &soundManager_, levelIndex, playerProfile_ ) );
            switchState( *level );
        } );
        layout->add(playLevelButton);
        layout->insertSpace(999, 0.5f);
    }

    tgui::Button::Ptr exitButton = std::make_shared<tgui::Button>();
    exitButton->setText( "Exit" );
    exitButton->setTextSize( 20 );
    exitButton->connect( "pressed", [this](){ requestStateExit(); } );
    layout->add(exitButton);

    gui.add(layout);
}


} // namespace jdb
