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

#include <level_ui/level_ui.hpp>
#include <paths.hpp>

namespace jdb {

LevelUI::LevelUI(PlayerHPGetter playerHpGetter, m2g::TilesetPtr playerHpPanel) :
    playerHpGetter_(playerHpGetter),
    playerHpPanel_(std::move(playerHpPanel))
{
    font_.loadFromFile(DATA_DIR_PATH + "/fonts/LiberationSans-Bold.ttf");

    playerHpText_.setFont( font_ );
    playerHpText_.setCharacterSize( 50 );
    playerHpText_.setColor( sf::Color( 131, 60, 60, 255 ) );
    playerHpText_.setPosition( 75, 5 );

    LevelUI::update();
}


/***
 * Updating
 ***/

void LevelUI::update()
{
    char buffer[10];
    sprintf( buffer, "%03d", playerHpGetter_() );
    playerHpText_.setString( buffer );
}


/***
 * Drawable interface
 ***/

void LevelUI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    playerHpPanel_.draw(target, states);
    target.draw(playerHpText_, states);
}

} // namespace jdb
