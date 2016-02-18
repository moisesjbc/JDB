#include <dangers/danger_data_parser.hpp>
#include <gtest/gtest.h>


TEST(DangerDataParserTest, StateTransitionIsParsedCorrectly) {
    json rawJSON = R"(
    {
        "state_transition": {
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
