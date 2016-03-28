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
    profile.updateSurvivalRecordScore(60);
    profile.updateCampaignLevelRecordScore(0, 10);
    profile.updateCampaignLevelRecordScore(1, 15);
    profile.updateGameDifficulty(jdb::GameDifficulty::HARD);
    json profileJSON = profileParser.writeToJSON(profile);

    EXPECT_EQ("profile-name", profileJSON["name"]);
    EXPECT_EQ(60, profileJSON["survival_record_score"]);
    EXPECT_EQ(10, profileJSON["campaign_record_scores"][0]);
    EXPECT_EQ(15, profileJSON["campaign_record_scores"][1]);
    EXPECT_EQ("hard", profileJSON["game_difficulty"]);
}


TEST(ProfileJSONParserTest, ProfileIsReadFromJSON)
{
    json profileJSON = R"(
    {
        "name": "awesome-player",
        "survival_record_score": 13,
        "campaign_record_scores": [15, 32],
        "game_difficulty": "easy"
    }
    )"_json;

    jdb::ProfileJSONParser profileParser;
    jdb::Profile profile = profileParser.readFromJSON(profileJSON);

    EXPECT_EQ("awesome-player", profile.name());
    EXPECT_EQ(2, profile.nextCampaignLevel());
    EXPECT_EQ(13, profile.survivalRecordScore());
    EXPECT_EQ(15, profile.campaignLevelRecordScore(0));
    EXPECT_EQ(32, profile.campaignLevelRecordScore(1));
    EXPECT_EQ(jdb::GameDifficulty::EASY, profile.gameDifficulty());
}
