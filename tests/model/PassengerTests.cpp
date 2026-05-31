#include <gtest/gtest.h>
#include "../../src/model/Passenger.h"

TEST(PassengerTest, ConstructorInitializesFields)
{
    Passenger p(1, 2, 5, 100);

    EXPECT_EQ(1, p.getId());
    EXPECT_EQ(2, p.getStartFloor());
    EXPECT_EQ(5, p.getDestinationFloor());
    EXPECT_EQ(100, p.getRequestTime());
}

TEST(PassengerTest, PickupTimeInitiallyMinusOne)
{
    Passenger p(1, 1, 2, 0);

    EXPECT_EQ(-1, p.getPickupTime());
}

TEST(PassengerTest, ArrivalTimeInitiallyMinusOne)
{
    Passenger p(1, 1, 2, 0);

    EXPECT_EQ(-1, p.getArrivalTime());
}

TEST(PassengerTest, SetPickupTimeWorks)
{
    Passenger p(1, 1, 2, 0);

    p.setPickupTime(50);

    EXPECT_EQ(50, p.getPickupTime());
}

TEST(PassengerTest, SetArrivalTimeWorks)
{
    Passenger p(1, 1, 2, 0);

    p.setArrivalTime(100);

    EXPECT_EQ(100, p.getArrivalTime());
}

TEST(PassengerTest, WaitingTimeCalculatedCorrectly)
{
    Passenger p(1, 1, 2, 10);

    p.setPickupTime(30);

    EXPECT_EQ(20, p.getWaitingTime());
}

TEST(PassengerTest, WaitingTimeZeroIfNotPickedUp)
{
    Passenger p(1, 1, 2, 10);

    EXPECT_EQ(0, p.getWaitingTime());
}

TEST(PassengerTest, RideTimeCalculatedCorrectly)
{
    Passenger p(1, 1, 2, 10);

    p.setPickupTime(20);
    p.setArrivalTime(50);

    EXPECT_EQ(30, p.getRideTime());
}

TEST(PassengerTest, RideTimeZeroIfNotArrived)
{
    Passenger p(1, 1, 2, 10);

    p.setPickupTime(20);

    EXPECT_EQ(0, p.getRideTime());
}