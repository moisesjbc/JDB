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

#include <level_ui/survival_level_ui.hpp>

namespace jdb {

SurvivalLevelUI::SurvivalLevelUI( PlayerHPGetter playerHpGetter,
                                  m2g::TilesetPtr playerHpPanel,
                                  PlayerScoreGetter playerScoreGetter,
                                  m2g::TilesetPtr playerScorePanel,
                                  ToolIndexGetter toolIndexGetter,
                                  m2g::TilesetPtr toolSelector,
                                  TimeGetter timeGetter,
                                  m2g::TilesetPtr timePanel ) :
    LevelUI(playerHpGetter,
            std::move(playerHpPanel),
            playerScoreGetter,
            std::move(playerScorePanel),
            toolIndexGetter,
            std::move(toolSelector)),
    timeGetter_(timeGetter),
    timePanel_(std::move(timePanel))
{
    timePanel_.move( 367.0f, 0.0f );

    timeText_.setFont( font() );
    timeText_.setCharacterSize( 50 );
    timeText_.setColor( sf::Color( 8, 31, 126, 255 ) );
    timeText_.setPosition( 450, 3 );
}


/***
 * LevelUI interface
 ***/

void SurvivalLevelUI::update()
{
    LevelUI::update();

    char buffer[10];

    // Compute the current game time.
    unsigned int seconds = timeGetter_();
    unsigned int minutes = seconds / 60;
    seconds = seconds % 60;

    sprintf( buffer, "%02d:%02d", minutes, seconds );
    timeText_.setString( buffer );
}


/***
 * Drawable interface
 ***/

void SurvivalLevelUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    LevelUI::draw(target, states);

    timePanel_.draw(target, states);

    target.draw(timeText_, states);
}

} // namespace jdb
