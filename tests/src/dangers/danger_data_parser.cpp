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
        "consequences": {
            "danger_hp_variation": -15,
            "new_state": 3,
            "new_danger": 5,
            "player_hp_bonus": 15
        }
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::PlayerActionResponse playerActionResponse = dangerDataParser.ParsePlayerActionResponse(rawJSON);

    EXPECT_EQ(jdb::PlayerAction::EXTINGUISHER_ON, playerActionResponse.playerAction);
    EXPECT_EQ(10, playerActionResponse.minHp);
    EXPECT_EQ(100, playerActionResponse.maxHp);
    EXPECT_EQ(-15, playerActionResponse.hpVariation);
    EXPECT_EQ(3, playerActionResponse.newState);
    EXPECT_EQ(5, playerActionResponse.newDanger);
    EXPECT_EQ(15, playerActionResponse.hpBonus);
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


TEST(DangerDataParserTest, StateTimeTransitionIsParsedCorrectly)
{
    json rawStateTimeTransitionJSON = R"(
    {
        "min_timeout": 7,
        "max_timeout": 9,
        "new_state": 3
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::StateTimeTransition stateTimeTransition = dangerDataParser.ParseStateTimeTransition(rawStateTimeTransitionJSON);

    EXPECT_EQ(7, stateTimeTransition.minTimeout);
    EXPECT_EQ(9, stateTimeTransition.maxTimeout);
    EXPECT_EQ(3, stateTimeTransition.newState);
}


TEST(DangerDataParserTest, StateDistanceTransitionIsParsedCorrectly)
{
    json rawDistanceTimeTransitionJSON = R"(
    {
        "distance": 32,
        "new_state": 5
    }
    )"_json;

    jdb::DangerDataParser dangerDataParser;
    jdb::StateDistanceTransition stateDistanceTransition =
            dangerDataParser.ParseStateDistanceTransition(rawDistanceTimeTransitionJSON);

    EXPECT_EQ(32, stateDistanceTransition.distance);
    EXPECT_EQ(5, stateDistanceTransition.newState);
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
                "consequences": {
                    "danger_hp_variation": -15,
                    "new_state": 3,
                    "new_danger": 5,
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
