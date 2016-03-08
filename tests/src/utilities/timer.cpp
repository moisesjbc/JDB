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

#include <utilities/timer.hpp>
#include <gtest/gtest.h>


TEST(TimerTest, IsInitializedToZero) {
    jdb::Timer timer;
    EXPECT_EQ(0, timer.seconds());
}


TEST(TimerTest, IsIncreasedCorrectly){
    jdb::Timer timer;
    EXPECT_EQ(0, timer.seconds());
    timer.update(500);
    EXPECT_EQ(0, timer.seconds());
    timer.update(500);
    EXPECT_EQ(1, timer.seconds());
    timer.update(2000);
    EXPECT_EQ(3, timer.seconds());
}


TEST(TimerTest, IsResetCorreclyWithoutArguments){
    jdb::Timer timer;
    timer.update(1000);
    timer.reset();
    EXPECT_EQ(0, timer.seconds());
}
