#include <profiles/profile.hpp>
#include <gtest/gtest.h>
#include <vector>

TEST(ProfileTest, nameIsInitializedInConstructor)
{
    std::vector<std::string> names =
        {"foo", "bar", "name with spaces"};

    for(const std::string& name : names){
        jdb::Profile profile(name);
        EXPECT_EQ(name, profile.name());
    }
}


TEST(ProfileTest, profileNamesWithLessThanThreeCharactersAreNotAllowed)
{
    std::vector<std::string> names =
        {"", "a", "ab"};

    for(const std::string& name : names){
        try{
            jdb::Profile profile(name);
            FAIL() << "Expected std::invalid_argument bacause of small name";
        }catch(std::invalid_argument& e){
            EXPECT_EQ("Profile name with less than 3 characters not allowed", std::string(e.what()));
        }catch(...){
            FAIL() << "Expected std::invalid_argument bacause of small name";
        }
    }
}


TEST(ProfileTest, nextCampaignLevelIsInitializedToZero)
{
    jdb::Profile profile("profile-name");
    EXPECT_EQ(0, profile.nextCampaignLevel());
}
