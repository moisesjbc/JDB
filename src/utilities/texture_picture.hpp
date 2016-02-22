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

#ifndef TEXTUREPICTURE_HPP
#define TEXTUREPICTURE_HPP

#include <TGUI/Widgets/Picture.hpp>

namespace jdb {

class TexturePicture : public tgui::Picture
{
    public:
        typedef std::shared_ptr<TexturePicture> Ptr;
        typedef std::shared_ptr<const TexturePicture> ConstPtr;


        /***
         * 1. Construction
         ***/
        static Ptr create( const std::string &filename,
                           bool fullyClickable = true );


        /***
         * 2. Setters
         ***/
        void setTexture( tgui::Texture texture );
};

} // namespace jdb

#endif // TEXTUREPICTURE_HPP
