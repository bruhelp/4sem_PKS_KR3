#include <gtest/gtest.h>
#include "../src/model/Call.h"

// Test constructor and getters with various inputs
TEST(CallTest, ConstructorInitializesCorrectly)
{
    Call call(101, 3, 7, 1000);
    EXPECT_EQ(101, call.getPassengerId());
    EXPECT_EQ(3, call.getFromFloor());
    EXPECT_EQ(7, call.getToFloor());
    EXPECT_EQ(1000, call.getTimestamp());
}

TEST(CallTest, PassengerIdCanBeZero)
{
    Call call(0, 1, 5, 10);
    EXPECT_EQ(0, call.getPassengerId());
}

TEST(CallTest, FromFloorEqualsToFloorAllowed)
{
    Call call(5, 4, 4, 200);
    EXPECT_EQ(4, call.getFromFloor());
    EXPECT_EQ(4, call.getToFloor());
}

TEST(CallTest, NegativeFloorsAllowed)
{
    Call call(1, -2, -1, 500);
    EXPECT_EQ(-2, call.getFromFloor());
    EXPECT_EQ(-1, call.getToFloor());
}

TEST(CallTest, TimestampCanBeLarge)
{
    Call call(2, 10, 20, 9999999);
    EXPECT_EQ(9999999, call.getTimestamp());
}

TEST(CallTest, ZeroTimestampIsValid)
{
    Call call(3, 1, 2, 0);
    EXPECT_EQ(0, call.getTimestamp());
}

TEST(CallTest, DifferentCombinationsWork)
{
    for (int i = 0; i < 5; ++i)
    {
        Call call(i, i + 1, i + 2, i * 100);
        EXPECT_EQ(i, call.getPassengerId());
        EXPECT_EQ(i + 1, call.getFromFloor());
        EXPECT_EQ(i + 2, call.getToFloor());
        EXPECT_EQ(i * 100, call.getTimestamp());
    }
}