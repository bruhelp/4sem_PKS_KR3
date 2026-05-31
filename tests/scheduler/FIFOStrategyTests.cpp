#include <gtest/gtest.h>
#include "../src/scheduler/FIFOStrategy.h"
#include "../src/core/ElevatorSystem.h"
#include "../src/core/Elevator.h"
#include "../src/model/Call.h"

class FIFOStrategyTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        system = new ElevatorSystem(nullptr, nullptr);
        strategy = std::make_unique<FIFOStrategy>();
    }

    void TearDown() override
    {
        delete system;
    }

    std::unique_ptr<FIFOStrategy> strategy;
    ElevatorSystem* system;
};

TEST_F(FIFOStrategyTest, AssignsToElevatorWithFewestPendingStops)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    e1->addDestination(3);
    e1->addDestination(4);
    e2->addDestination(5);
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 1, 2, 0);
    Elevator* chosen = strategy->assign(*system, call);
    EXPECT_EQ(2, chosen->getId()); // e2 has 1 stop vs e1 has 2
}

TEST_F(FIFOStrategyTest, WhenEqualStopsPicksFirstEncountered)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    e1->addDestination(1);
    e2->addDestination(2);
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 1, 2, 0);
    Elevator* chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); // first elevator with minimal stops (both have 1)
}

TEST_F(FIFOStrategyTest, EmptyPendingStopsSelectsAny)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 1, 2, 0);
    Elevator* chosen = strategy->assign(*system, call);
    EXPECT_NE(nullptr, chosen);
}

TEST_F(FIFOStrategyTest, IgnoresDirectionAndDistance)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    e1->addDestination(10); // far but few stops
    e2->addDestination(2);
    e2->addDestination(3);
    e2->addDestination(4); // many stops but near
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 1, 2, 0);
    Elevator* chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); // e1 has 1 stop < e2's 3 stops
}

TEST_F(FIFOStrategyTest, NameReturnsFIFO)
{
    EXPECT_STREQ("FIFO", strategy->getName());
}