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

#ifndef SURVIVAL_LEVEL_UI
#define SURVIVAL_LEVEL_UI

#include <level_ui/level_ui.hpp>

namespace jdb {

typedef std::function<unsigned int()> TimeGetter;

class SurvivalLevelUI : public LevelUI
{
    public:
        /***
         * Construction
         ***/
        SurvivalLevelUI(PlayerHPGetter playerHpGetter,
                        m2g::TilesetPtr playerHpPanel,
                        PlayerScoreGetter playerScoreGetter,
                        m2g::TilesetPtr playerScorePanel,
                        ToolIndexGetter toolIndexGetter,
                        m2g::TilesetPtr toolSelector,
                        TimeGetter timeGetter,
                        m2g::TilesetPtr timePanel);


        /***
         * LevelUI interface
         ***/
        void update();


    protected:
        /***
         * Drawable interface
         ***/
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    public:
        /***
         * Attributes
         ***/
        TimeGetter timeGetter_;
        m2g::TileSprite timePanel_;
        sf::Text timeText_;
};

} // namespace jdb

#endif // SURVIVAL_LEVEL_UI
