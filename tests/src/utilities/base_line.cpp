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

#include <utilities/base_line.hpp>
#include <tinyxml2.h>
#include <gtest/gtest.h>


TEST(BaseLineTest, IsInitializedToZero) {
    jdb::BaseLine baseLine;

    EXPECT_EQ(0.0f, baseLine.x);
    EXPECT_EQ(0.0f, baseLine.y);
    EXPECT_EQ(0.0f, baseLine.width);
}


TEST(BaseLineTest, LoadsFromValidXML) {
    // Build a basic XML defining a base line and parse it.
    const char* xmlContent =
        "<base_line x=\"15.5\" y=\"31\" width=\"0.3\"></base_line>";
    tinyxml2::XMLDocument doc;
    doc.Parse(xmlContent);

    // Read the base line from previous XML.
    jdb::BaseLine baseLine;
    baseLine.loadFromXML(doc.FirstChildElement("base_line"));

    // Test results
    EXPECT_EQ(15.5f, baseLine.x);
    EXPECT_EQ(31, baseLine.y);
    EXPECT_EQ(0.3f, baseLine.width);
}


TEST(BaseLineTest, MissingValuesFromXMLAreSetToZero) {
    // Build a basic XML defining a base line and parse it.
    const char* xmlContent =
        "<base_line y=\"31\" width=\"0.3\"></base_line>";
    tinyxml2::XMLDocument doc;
    doc.Parse(xmlContent);

    // Read the base line from previous XML.
    jdb::BaseLine baseLine;
    baseLine.loadFromXML(doc.FirstChildElement("base_line"));

    EXPECT_EQ(0.0f, baseLine.x);
    EXPECT_EQ(31, baseLine.y);
    EXPECT_EQ(0.3f, baseLine.width);
}
