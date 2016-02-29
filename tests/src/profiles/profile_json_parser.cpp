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

#include <profiles/profile_json_parser.hpp>
#include <gtest/gtest.h>


TEST(ProfileJSONParserTest, ProfileIsWrittenToJSON)
{
    jdb::ProfileJSONParser profileParser;
    jdb::Profile profile("profile-name");
    json profileJSON = profileParser.writeToJSON(profile);

    EXPECT_EQ("profile-name", profileJSON["name"]);
    EXPECT_EQ(0, profileJSON["next_campaign_level"]);
}


TEST(ProfileJSONParserTest, ProfileIsReadFromJSON)
{
    json profileJSON = R"(
    {
        "name": "awesome-player",
        "next_campaign_level": 5
    }
    )"_json;

    jdb::ProfileJSONParser profileParser;
    jdb::Profile profile = profileParser.readFromJSON(profileJSON);

    EXPECT_EQ("awesome-player", profile.name());
    EXPECT_EQ(5, profile.nextCampaignLevel());
}
