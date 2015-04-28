/***
    Copyright 2013 - 2015 Moises J. Bonilla Caraballo (Neodivert)

    This file is part of JDB.

    JDB is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    JDB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with JDB.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#ifndef LEVELBOOK_HPP
#define LEVELBOOK_HPP

#include <TGUI/Gui.hpp>
#include <TGUI/Panel.hpp>
#include <TGUI/SharedWidgetPtr.hpp>
#include <vector>
#include <string>
#include <TGUI/Scrollbar.hpp>
#include <TGUI/TextBox.hpp>
#include <TGUI/Button.hpp>

namespace jdb {

// TODO: Add copy constructor and clone().
class LevelBook : public tgui::Panel
{
    public:
        typedef std::shared_ptr<LevelBook> Ptr;
        typedef std::shared_ptr<const LevelBook> ConstPtr;


        /***
         * 1. Construction
         ***/
        LevelBook();
        LevelBook( const LevelBook& b );
        static LevelBook::Ptr create();


        /***
         * 2. Destruction
         ***/
        virtual ~LevelBook() = default;


        /***
         * 3. Pages management
         ***/
        void addPage( const sf::String& text );


        /***
         * 4. Setters
         ***/
        void setPage( unsigned int pageIndex );
        void setNextPage();


    private:
        unsigned int currentPageIndex_;
        std::vector< std::string > pagesText_;
        tgui::TextBox::Ptr textBox_;
        tgui::Button::Ptr continueButton_;
};


} // namespace jdb

#endif // LEVELBOOK_HPP
