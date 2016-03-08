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

#ifndef LEVEL_UI_HPP
#define LEVEL_UI_HPP


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <m2g/drawables/tile_sprite.hpp>
#include <functional>
#include <SFML/Graphics/Text.hpp>

namespace jdb {

typedef std::function<int()> PlayerHPGetter;
typedef std::function<int()> PlayerScoreGetter;
typedef std::function<unsigned int()> ToolIndexGetter;

class LevelUI : public sf::Drawable
{
    public:
        /***
         * Construction
         ***/
        LevelUI(PlayerHPGetter playerHpGetter,
                m2g::TilesetPtr playerHpPanel,
                PlayerScoreGetter playerScoreGetter,
                m2g::TilesetPtr playerScorePanel,
                ToolIndexGetter toolIndexGetter,
                m2g::TilesetPtr toolSelector);


        /***
         * Getters
         ***/
        const sf::Font& font() const;


        /***
         * LevelListener interface
         ***/
        virtual void update();


    protected:
        /***
         * Drawable interface
         ***/
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;


    private:
        /***
         * Private Attributes
         ***/
        PlayerHPGetter playerHpGetter_;
        m2g::TileSprite playerHpPanel_;
        sf::Text playerHpText_;

        PlayerHPGetter playerScoreGetter_;
        m2g::TileSprite playerScorePanel_;
        sf::Text playerScoreText_;

        ToolIndexGetter toolIndexGetter_;
        m2g::TileSprite toolSelector_;
        sf::Font font_;
};

} // namespace jdb

#endif // LEVEL_UI_HPP
