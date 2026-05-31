#include <gtest/gtest.h>
#include "../src/services/Config.h"

TEST(ConfigTest, IsValidReturnsTrueForTypicalValues)
{
    Config cfg;
    cfg.floors = 10;
    cfg.elevators = 3;
    cfg.capacity = 8;
    cfg.passengersPerFloor = 10;
    cfg.simulationDuration = 300;
    EXPECT_TRUE(cfg.isValid());
}

TEST(ConfigTest, IsValidFalseWhenFloorsBelowMin)
{
    Config cfg;
    cfg.floors = 1;
    EXPECT_FALSE(cfg.isValid());
}

TEST(ConfigTest, IsValidFalseWhenFloorsAboveMax)
{
    Config cfg;
    cfg.floors = 51;
    EXPECT_FALSE(cfg.isValid());
}

TEST(ConfigTest, IsValidFalseWhenElevatorsBelowMin)
{
    Config cfg;
    cfg.floors = 10;
    cfg.elevators = 0;
    EXPECT_FALSE(cfg.isValid());
}

TEST(ConfigTest, IsValidFalseWhenElevatorsAboveMax)
{
    Config cfg;
    cfg.elevators = 7;
    EXPECT_FALSE(cfg.isValid());
}

TEST(ConfigTest, IsValidFalseWhenCapacityOutOfRange)
{
    Config cfg;
    cfg.capacity = 0;
    EXPECT_FALSE(cfg.isValid());
    cfg.capacity = 21;
    EXPECT_FALSE(cfg.isValid());
}

TEST(ConfigTest, IsValidFalseWhenPassengersPerFloorOutOfRange)
{
    Config cfg;
    cfg.passengersPerFloor = 0;
    EXPECT_FALSE(cfg.isValid());
    cfg.passengersPerFloor = 51;
    EXPECT_FALSE(cfg.isValid());
}

TEST(ConfigTest, IsValidFalseWhenSimulationDurationNonPositive)
{
    Config cfg;
    cfg.simulationDuration = 0;
    EXPECT_FALSE(cfg.isValid());
    cfg.simulationDuration = -10;
    EXPECT_FALSE(cfg.isValid());
}

TEST(ConfigTest, IsValidTrueAtBoundaries)
{
    Config cfg;
    cfg.floors = 2;
    cfg.elevators = 1;
    cfg.capacity = 1;
    cfg.passengersPerFloor = 1;
    cfg.simulationDuration = 1;
    EXPECT_TRUE(cfg.isValid());

    cfg.floors = 50;
    cfg.elevators = 6;
    cfg.capacity = 20;
    cfg.passengersPerFloor = 50;
    EXPECT_TRUE(cfg.isValid());
}