#include <profiles/profile.hpp>
#include <gtest/gtest.h>
#include <vector>

TEST(ProfileTest, nameIsInitializedInConstructor) {
    std::vector<std::string> names =
        {"foo", "bar", "name with spaces"};

    for(const std::string& name : names){
        jdb::Profile profile(name);
        EXPECT_EQ(name, profile.name());
    }
}
