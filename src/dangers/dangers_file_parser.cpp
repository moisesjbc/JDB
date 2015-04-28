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

#include "dangers_file_parser.hpp"
#include <stdexcept>

namespace jdb {

/***
 * 1. Construction
 ***/

DangersFileParser::DangersFileParser(const std::string &filePath)
{
    if( file_.LoadFile( filePath.c_str() ) != tinyxml2::XML_NO_ERROR ){
        throw std::runtime_error( file_.GetErrorStr1() );
    }
}


/***
 * 2. Parsing
 ***/

std::vector<DangerInfo> DangersFileParser::getLevelDangersInfo(unsigned int level)
{
    std::vector<DangerInfo> dangersInfo;

    tinyxml2::XMLElement* dangerXmlElement =
            file_.RootElement()->FirstChildElement("danger");

    while( dangerXmlElement ){
        if( dangerXmlElement->UnsignedAttribute("first_level") == level ){
            dangersInfo.push_back( getDangerInfo( dangerXmlElement ) );
        }
        dangerXmlElement = dangerXmlElement->NextSiblingElement("danger");
    }

    return dangersInfo;
}


/***
 * 3. Parsing auxiliar methods
 ***/

DangerInfo DangersFileParser::getDangerInfo(tinyxml2::XMLElement *dangerXmlElement)
{
    DangerInfo dangerInfo;

    tinyxml2::XMLElement* dangerInfoXmlElement =
            dangerXmlElement->FirstChildElement("danger_info");

    if( dangerInfoXmlElement != nullptr ){
        dangerInfo.name = dangerInfoXmlElement->FirstChildElement("name")->GetText();
        dangerInfo.description = dangerInfoXmlElement->FirstChildElement("description")->GetText();
        dangerInfo.removalInstructions = dangerInfoXmlElement->FirstChildElement("removal_instructions")->GetText();
    }else{
        dangerInfo.name = "UNDEFINED";
        dangerInfo.description = "UNDEFINED";
        dangerInfo.removalInstructions = "UNDEFINED";
    }

    return dangerInfo;
}




} // namespace jdb

