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

    playLevelButtons_.reserve(nLevels_);

    layout->add(generateGameDifficultySelector());
    layout->insertSpace(999, 0.5f);

    for(unsigned int levelIndex=0; levelIndex<nLevels_; levelIndex++){
        tgui::Button::Ptr playLevelButton = std::make_shared<tgui::Button>();
        playLevelButton->setTextSize( 20 );

        playLevelButton->setText( "Play level " + std::to_string(levelIndex) );
        playLevelButton->connect( "pressed", [=](){
            std::unique_ptr< Level > level = std::unique_ptr< Level >(
                       new CampaignLevel( window_, &soundManager_, levelIndex, playerProfile_ ) );
            switchState( *level );
        } );

        if(levelIndex > playerProfile_.nextCampaignLevel()){
            playLevelButton->setText( playLevelButton->getText() + " [LOCKED]" );
            playLevelButton->disable();
        }else{
            tgui::Label::Ptr toolTip = std::make_shared<tgui::Label>();
            toolTip->setText("Current record score: " + std::to_string(playerProfile_.campaignLevelRecordScore(levelIndex)));
            playLevelButton->setToolTip(toolTip);
        }

        layout->add(playLevelButton);
        playLevelButtons_.push_back(playLevelButton);
        layout->insertSpace(999, 0.5f);
    }

    tgui::Button::Ptr exitButton = std::make_shared<tgui::Button>();
    exitButton->setText( "Exit" );
    exitButton->setTextSize( 20 );
    exitButton->connect( "pressed", [this](){ requestStateExit(); } );
    layout->add(exitButton);

    gui.add(layout);
}


/***
 * GameState interface
 ***/

void CampaignLevelSelectionMenu::resume()
{
    GUIMenu::resume();

    // If player unlocked levels before returning to this menu,
    // unlock their respective buttons.
    for(unsigned int levelIndex=0; levelIndex<nLevels_; levelIndex++){
        if(!playLevelButtons_[levelIndex]->isEnabled() && levelIndex <= playerProfile_.nextCampaignLevel()){
            std::string buttonText = playLevelButtons_[levelIndex]->getText().toAnsiString();
            playLevelButtons_[levelIndex]->setText(buttonText.substr(0, buttonText.size() - std::string(" [LOCKED]").size() ));
            tgui::Label::Ptr toolTip = std::make_shared<tgui::Label>();
            toolTip->setText("Current record score: " + std::to_string(playerProfile_.campaignLevelRecordScore(levelIndex)));
            playLevelButtons_[levelIndex]->setToolTip(toolTip);
            playLevelButtons_[levelIndex]->enable();
        }
    }
}


/***
 * Auxiliar initializations
 ***/

tgui::Widget::Ptr CampaignLevelSelectionMenu::generateGameDifficultySelector()
{
    tgui::HorizontalLayout::Ptr layout =
        std::make_shared<tgui::HorizontalLayout>();

    tgui::ComboBox::Ptr gameDifficultySelector =
        std::make_shared<tgui::ComboBox>();
    gameDifficultySelector->addItem("GameDifficulty: Easy", "easy");
    gameDifficultySelector->addItem("GameDifficulty: Normal", "normal");
    gameDifficultySelector->addItem("GameDifficulty: Hard", "hard");

    gameDifficultySelector->connect("ItemSelected", [this](sf::String text, sf::String itemID){
        if(itemID.toAnsiString() == "easy"){
            playerProfile_.updateGameDifficulty(GameDifficulty::EASY);
        }else if(itemID.toAnsiString() == "normal"){
            playerProfile_.updateGameDifficulty(GameDifficulty::NORMAL);
        }else{
            playerProfile_.updateGameDifficulty(GameDifficulty::HARD);
        }
        Level::savePlayerProfile(playerProfile_);
    });

    switch(playerProfile_.gameDifficulty()){
        case GameDifficulty::EASY:
            gameDifficultySelector->setSelectedItemById("easy");
        break;
        case GameDifficulty::NORMAL:
            gameDifficultySelector->setSelectedItemById("normal");
        break;
        default:
            gameDifficultySelector->setSelectedItemById("hard");
        break;
    }

    gameDifficultySelector->setTextSize(20);
    layout->add(gameDifficultySelector);

    return layout;
}


} // namespace jdb
