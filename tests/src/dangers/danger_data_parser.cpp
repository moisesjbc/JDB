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

#include <dangers/danger_data_parser.hpp>
#include <gtest/gtest.h>
#include <paths.hpp>

TEST(DangerDataParserTest, StateTransitionIsParsedCorrectly) {
    json rawJSON = R"(
    {
        "conditions": {
            "player_action": "extinguisher_on",
            "danger_min_hp": 10,
            "danger_max_hp": 100
        },
        "danger_mutation": {
            "danger_hp_variation": -15,
            "new_state": 3,
            "new_danger": {
                "id": "fire"
            },
            "player_hp_bonus": 15
        }
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::PlayerActionResponse playerActionResponse = dangerDataParser.ParsePlayerActionResponse(rawJSON);

    EXPECT_EQ(jdb::PlayerAction::EXTINGUISHER_ON, playerActionResponse.playerAction);
    EXPECT_EQ(10, playerActionResponse.minHp);
    EXPECT_EQ(100, playerActionResponse.maxHp);
}


TEST(DangerDataParserTest, BaseLineIsParsedCorrectly)
{
    json rawBaseLineJSON = R"(
    {
        "x": 16,
        "y": 7,
        "width": 275
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::BaseLine baseLine = dangerDataParser.ParseBaseLine(rawBaseLineJSON);

    EXPECT_EQ(16, baseLine.x);
    EXPECT_EQ(7, baseLine.y);
    EXPECT_EQ(275, baseLine.width);
}


TEST(DangerDataParserTest, DangerMutationIsParsedCorrectly)
{
    json rawStateTimeTransitionJSON = R"(
    {
        "danger_hp_variation": -15,
        "new_danger_state": 3,
        "new_danger": {
            "id": "fire",
            "appearance_animation": "boo.png"
        },
        "player_hp_variation": 15,
        "player_score_variation": -5
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::DangerMutation dangerMutation = dangerDataParser.ParseDangerMutation(rawStateTimeTransitionJSON);

    EXPECT_EQ(-15, dangerMutation.dangerHpVariation());
    EXPECT_EQ(3, dangerMutation.newDangerState());
    EXPECT_EQ("fire", dangerMutation.newDanger());
    EXPECT_EQ("boo.png", dangerMutation.newDangerAppearanceAnimaton());
    EXPECT_EQ(15, dangerMutation.playerHpVariation());
    EXPECT_EQ(-5, dangerMutation.playerScoreVariation());
}


TEST(DangerDataParserTest, DangerMutationUndefinedValuesAreGivenDefaults)
{
    json rawStateTimeTransitionJSON = R"(
    {
        "danger_hp_variation": -15
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::DangerMutation dangerMutation = dangerDataParser.ParseDangerMutation(rawStateTimeTransitionJSON);

    EXPECT_EQ(-15, dangerMutation.dangerHpVariation());
    EXPECT_EQ(0, dangerMutation.newDangerState());
    EXPECT_EQ("", dangerMutation.newDanger());
    EXPECT_EQ("", dangerMutation.newDangerAppearanceAnimaton());
    EXPECT_EQ(0, dangerMutation.playerHpVariation());
    EXPECT_EQ(0, dangerMutation.playerScoreVariation());
}


TEST(DangerDataParserTest, StateDistanceTransitionIsParsedCorrectly)
{
    json rawDistanceTimeTransitionJSON = R"(
    {
        "distance": 32,
        "danger_mutation": {
            "new_danger_state": 5
        }
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::StateDistanceTransition stateDistanceTransition =
            dangerDataParser.ParseStateDistanceTransition(rawDistanceTimeTransitionJSON);

    EXPECT_EQ(32, stateDistanceTransition.distance);
}


TEST(DangerDataParserTest, DangerStateStunIsParsedCorrectly)
{
    json rawDangerStateStunJSON = R"(
    {
        "type": "burn",
        "tools": ["hand", "gavel"]
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::StunType stunType;
    std::set<jdb::ToolType> stunnedTools;
    dangerDataParser.ParseDangerStateStun(rawDangerStateStunJSON, stunType, stunnedTools);

    EXPECT_EQ(jdb::StunType::BURN, stunType);
    EXPECT_NE(stunnedTools.end(), stunnedTools.find(jdb::ToolType::HAND));
    EXPECT_NE(stunnedTools.end(), stunnedTools.find(jdb::ToolType::GAVEL));
    EXPECT_EQ(stunnedTools.end(), stunnedTools.find(jdb::ToolType::EXTINGUISHER));
    EXPECT_EQ(stunnedTools.end(), stunnedTools.find(jdb::ToolType::LIGHTER));
}


TEST(DangerDataParserTest, DangerStateIsParsedCorrectly)
{
    json rawDangerStateJSON = R"(
    {
        "animation_state": 1,
        "player_action_responses": [
            {
                "conditions": {
                    "player_action": "extinguisher_on",
                    "danger_min_hp": 10,
                    "danger_max_hp": 100
                },
                "danger_mutation": {
                    "danger_hp_variation": -15,
                    "new_state": 3,
                    "new_danger": {
                        "id": "fire"
                    },
                    "player_hp_bonus": 15
                }
            }
        ]
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    m2g::GraphicsLibrary dangersGraphics(jdb::DATA_DIR_PATH + "/img/dangers/dangers.xml");
    jdb::DangerState dangerState =
        dangerDataParser.ParseDangerState(rawDangerStateJSON, dangersGraphics);

    EXPECT_EQ(1, dangerState.animationState);
}


TEST(DangerDataParserTest, DangerInfoIsParsedCorrectly)
{
    json rawDangerInfoJSON = R"(
    {
        "name": "Fire",
        "description": "A fire",
        "removal_instructions": "Don't touch it!"
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::DangerInfo dangerInfo =
            dangerDataParser.ParseDangerInfo("fire", rawDangerInfoJSON);

    EXPECT_EQ("Fire", dangerInfo.name);
    EXPECT_EQ("A fire", dangerInfo.description);
    EXPECT_EQ("Don't touch it!", dangerInfo.removalInstructions);
}
