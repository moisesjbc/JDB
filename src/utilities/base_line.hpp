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

#ifndef BASE_LINE_HPP
#define BASE_LINE_HPP

#include <tinyxml2.h>

namespace jdb {

struct BaseLine
{
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;


    /*** Methods ***/

    /***
     * 1. Loading
     ***/
    void loadFromXML( tinyxml2::XMLElement* xmlElement );
    BaseLine(float x = 0.0f, float y = 0.0f, float width = 0.0f);
};

}

#endif // BASE_LINE_HPP
