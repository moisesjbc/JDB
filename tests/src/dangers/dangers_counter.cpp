#include <dangers/dangers_counter.hpp>
#include <stdexcept>
#include <gtest/gtest.h>

TEST(DangersCounterTest, nDangersIsInitializedInConstructor) {
    std::vector<int> nDangersVector = {5, 1, 15};
    for( int nDangers : nDangersVector ){
        jdb::DangersCounter dangersFactoryA(nDangers);
        EXPECT_EQ(nDangers, dangersFactoryA.nDangers());
    }
}


TEST(DangersCounterTest, CantCreateEmptyDangersCounter){
    try{
        jdb::DangersCounter emptyDangersCounter(0);
        FAIL() << "Expected std::out_of_range";
    }catch(std::out_of_range const&){
        SUCCEED();
    }catch(...){
        FAIL() << "Expected std::out_of_range";
    }
}
