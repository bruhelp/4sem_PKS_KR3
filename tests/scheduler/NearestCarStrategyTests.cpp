// tests/scheduler/NearestCarStrategyTests.cpp
#include <gtest/gtest.h>
#include "../src/scheduler/NearestCarStrategy.h"
#include "../src/core/ElevatorSystem.h"
#include "../src/core/Elevator.h"
#include "../src/model/Call.h"

class NearestCarStrategyTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        system = new ElevatorSystem(nullptr, nullptr);
        strategy = std::make_unique<NearestCarStrategy>();
    }

    void TearDown() override
    {
        delete system;
    }

    std::unique_ptr<NearestCarStrategy> strategy;
    ElevatorSystem* system;
};

TEST_F(NearestCarStrategyTest, AssignsClosestElevatorByCurrentFloor)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    
    // Move e1 to floor 3
    e1->addDestination(3);
    while (e1->getCurrentFloor() < 3) e1->moveOneFloor();
    
    // Move e2 to floor 8
    e2->addDestination(8);
    while (e2->getCurrentFloor() < 8) e2->moveOneFloor();
    
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    
    Call call(1, 5, 10, 0); // from floor 5
    Elevator* chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); // elevator1 at floor 3 (distance 2) vs elevator2 at 8 (distance 3)
}

TEST_F(NearestCarStrategyTest, TieBreakFirstEncountered)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    
    // Move both to floor 5
    e1->addDestination(5);
    while (e1->getCurrentFloor() < 5) e1->moveOneFloor();
    e2->addDestination(5);
    while (e2->getCurrentFloor() < 5) e2->moveOneFloor();
    
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    
    Call call(1, 5, 6, 0);
    Elevator* chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); // first elevator returned
}

TEST_F(NearestCarStrategyTest, WorksWithElevatorAtSameFloor)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    // e1 at floor 1 initially
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    
    e2->addDestination(10);
    while (e2->getCurrentFloor() < 10) e2->moveOneFloor();
    
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    
    Call call(1, 1, 2, 0);
    Elevator* chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); // elevator1 is at floor 1, distance 0
}

TEST_F(NearestCarStrategyTest, IgnoresPendingStops)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    
    // Move e1 to floor 2
    e1->addDestination(2);
    while (e1->getCurrentFloor() < 2) e1->moveOneFloor();
    
    e2->addDestination(100); // many pending stops but far away
    while (e2->getCurrentFloor() < 50) e2->moveOneFloor();
    
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    
    Call call(1, 1, 2, 0);
    Elevator* chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); // closest by floor, not by pending stops
}

TEST_F(NearestCarStrategyTest, NameReturnsCorrectString)
{
    EXPECT_STREQ("Nearest Car", strategy->getName());
}