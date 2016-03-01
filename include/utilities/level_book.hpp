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

#ifndef LEVELBOOK_HPP
#define LEVELBOOK_HPP

#include <TGUI/Gui.hpp>
#include <TGUI/VerticalLayout.hpp>
#include <vector>
#include <string>
#include <TGUI/Widgets/Scrollbar.hpp>
#include <TGUI/Widgets/TextBox.hpp>
#include <TGUI/Widgets/Button.hpp>
#include "../utilities/texture_picture.hpp"

namespace jdb {

struct BookPage
{
    std::string text;
    std::unique_ptr< tgui::Texture > texture;

    BookPage( std::string text, std::unique_ptr< tgui::Texture > texture ) :
        text( text ),
        texture( std::move( texture ) )
    {}
};


class LevelBook : public tgui::VerticalLayout
{
    public:
        typedef std::shared_ptr<LevelBook> Ptr;
        typedef std::shared_ptr<const LevelBook> ConstPtr;


        /***
         * 1. Construction
         ***/
        LevelBook();


        /***
         * 2. Destruction
         ***/
        virtual ~LevelBook() = default;


        /***
         * 3. Pages management
         ***/
        void addPage( const sf::String& text,
                      std::unique_ptr< tgui::Texture > texture = nullptr );


        /***
         * 4. Setters
         ***/
        void setPage( unsigned int pageIndex );
        void setNextPage();


    private:
        unsigned int currentPageIndex_;
        std::vector< BookPage > pages_;
        tgui::TextBox::Ptr textBox_;
        TexturePicture::Ptr picture_;
        tgui::Button::Ptr continueButton_;
};


} // namespace jdb

#endif // LEVELBOOK_HPP
