#include <dangers/dangers_counter.hpp>
#include <stdexcept>
#include <gtest/gtest.h>

TEST(DangersCounterTest, nDangersIsInitializedInConstructor) {
    std::vector<int> nDangersVector = {5, 1, 15};
    for( int nDangers : nDangersVector ){
        jdb::DangersCounter dangersFactoryA(nDangers, {"danger"});
        EXPECT_EQ(nDangers, dangersFactoryA.nDangers());
    }
}


TEST(DangersCounterTest, CantCreateEmptyDangersCounter){
    try{
        jdb::DangersCounter emptyDangersCounter(0, {"danger"});
        FAIL() << "Expected std::out_of_range";
    }catch(std::out_of_range const&){
        SUCCEED();
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}


TEST(DangersCounterTest, AnUniqueDangerOccupiesTheFullCounter) {
    unsigned int nDangers = 15;
    jdb::DangersCounter dangersFactoryA(nDangers, {"danger"});
    EXPECT_EQ(nDangers, dangersFactoryA.nDangers("danger"));
}


TEST(DangersCounterTest, DangersTotalIsDistributedBetweenSpecificDangers) {
    unsigned int nDangers = 15;
    jdb::DangersCounter dangersFactory(nDangers, {"dangerA", "dangerB", "dangerC"});
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerA"));
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerB"));
    EXPECT_EQ(nDangers / 3, dangersFactory.nDangers("dangerC"));
}


TEST(DangersCounterTest, InvalidIDThrows) {
    const std::string expectedDangerID = "danger";
    try {
        jdb::DangersCounter dangersFactoryA(15, {expectedDangerID});
        dangersFactoryA.nDangers("another-danger");
        FAIL() << "Expected std::out_of_range from invalid danger ID";
    }catch(std::out_of_range const& e){
        EXPECT_NE(nullptr, strstr(e.what(), "another-danger")) << "Expected \"" + expectedDangerID + "\" in exception message";
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}
