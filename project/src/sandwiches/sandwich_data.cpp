/***
    Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)

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

#include "sandwich_data.hpp"

namespace jdb {


/***
 * 1. Initialization
 ***/

SandwichData::SandwichData( tinyxml2::XMLElement* xmlElement )
{
    loadFromXML( xmlElement );
}


/***
 * 2. Loading
 ***/

void SandwichData::loadFromXML( tinyxml2::XMLElement* xmlElement )
{
    tinyxml2::XMLElement* xmlWidthSegment;
    m2g::GraphicsLoader graphicsLoader;
    m2g::Rect contactRect;

    std::cout << "Loading SandwichData from XML: " << std::endl;

    graphicsLoader.loadAnimationData( animationData, "data/img/sandwiches", "sandwich_01.png" );

    xmlWidthSegment = xmlElement->FirstChildElement( "width_segment" );
    x0 = xmlWidthSegment->FloatAttribute( "x0" );
    x1 = xmlWidthSegment->FloatAttribute( "x1" );

    std::cout << "\tx0: " << x0 << ", x1: " << x1 << std::endl;

    xmlElement = xmlElement->FirstChildElement( "contact_rect" );
    while( xmlElement ){
        contactRect.loadFromXML( xmlElement );

        contactRects.push_back( contactRect );

        std::cout << "\tContact Rect: ( " << contactRects.back().x << ", " << contactRects.back().y << ", " << contactRects.back().width << ", " << contactRects.back().height << ")" << std::endl;

        xmlElement = xmlElement->NextSiblingElement( "contact_rect" );
    }
}


} // namespace jdb
