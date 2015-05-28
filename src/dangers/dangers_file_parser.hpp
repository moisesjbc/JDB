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

#ifndef DANGERSFILEPARSER_HPP
#define DANGERSFILEPARSER_HPP

#include <tinyxml2.h>
#include <string>
#include <vector>
#include <TGUI/Texture.hpp>
#include <paths.hpp>

namespace jdb {

struct DangerInfo
{
    std::string name;
    std::string description;
    std::string removalInstructions;

    std::string texturePath;
    sf::Vector2u textureSize;
};

class DangersFileParser
{
    public:
        /***
         * 1. Construction
         ***/
        DangersFileParser( const std::string& filePath );


        /***
         * 2. Parsing
         ***/
        std::vector< DangerInfo > getLevelDangersInfo( unsigned int level );


    private:
        /***
         * 3. Parsing auxiliar methods
         ***/
        DangerInfo getDangerInfo( tinyxml2::XMLElement* dangerXmlElement );
        sf::Vector2u getTileSize( std::string dangerName );


        /***
         * Attributes
         ***/
        tinyxml2::XMLDocument file_;
};

} // namespace jdb

#endif // DANGERSFILEPARSER_HPP
