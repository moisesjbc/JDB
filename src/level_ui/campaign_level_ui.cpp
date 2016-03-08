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

#include <level_ui/campaign_level_ui.hpp>

namespace jdb {

CampaignLevelUI::CampaignLevelUI( PlayerHPGetter playerHpGetter,
                                  m2g::TilesetPtr playerHpPanel,
                                  PlayerScoreGetter playerScoreGetter,
                                  m2g::TilesetPtr playerScorePanel,
                                  ToolIndexGetter toolIndexGetter,
                                  m2g::TilesetPtr toolSelector,
                                  ProgressGetter progressGetter,
                                  m2g::TilesetPtr progressPanel ) :
    LevelUI(playerHpGetter,
            std::move(playerHpPanel),
            playerScoreGetter,
            std::move(playerScorePanel),
            toolIndexGetter,
            std::move(toolSelector)),
    progressGetter_(progressGetter),
    progressPanel_(std::move(progressPanel))
{
    progressPanel_.move( 367.0f, 0.0f );

    progressText_.setFont( font() );
    progressText_.setCharacterSize( 50 );
    progressText_.setColor( sf::Color( 8, 31, 126, 255 ) );
    progressText_.setPosition( 450, 3 );
}


/***
 * LevelUI interface
 ***/

void CampaignLevelUI::update()
{
    LevelUI::update();

    char buffer[10];
    sprintf(buffer, "%4.1f%%", progressGetter_());
    progressText_.setString(buffer);
}


/***
 * Drawable interface
 ***/

void CampaignLevelUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    LevelUI::draw(target, states);

    progressPanel_.draw(target, states);

    target.draw(progressText_, states);
}

} // namespace jdb
