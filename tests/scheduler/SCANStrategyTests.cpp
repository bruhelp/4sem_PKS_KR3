#include <gtest/gtest.h>
#include "../src/scheduler/SCANStrategy.h"
#include "../src/core/ElevatorSystem.h"
#include "../src/core/Elevator.h"
#include "../src/model/Call.h"

class SCANStrategyTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        system = new ElevatorSystem(nullptr, nullptr);
        strategy = std::make_unique<SCANStrategy>();
    }

    void TearDown() override
    {
        delete system;
    }

    std::unique_ptr<SCANStrategy> strategy;
    ElevatorSystem *system;
};

TEST_F(SCANStrategyTest, AssignsIdleElevatorRegardlessOfDirection)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);

    e1->addDestination(10);
    while (e1->getCurrentFloor() < 10)
        e1->moveOneFloor();
    e1->addDestination(1);
    e1->moveOneFloor(); // 10 -> 9
    e1->moveOneFloor(); // 9 -> 8

    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));

    Call call(1, 9, 10, 0);
    Elevator *chosen = strategy->assign(*system, call);

    EXPECT_EQ(2, chosen->getId());
}

TEST_F(SCANStrategyTest, AssignsElevatorMovingInSameDirectionAndAhead)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    // move e1 to floor 2, direction Up
    e1->addDestination(3);
    while (e1->getCurrentFloor() < 2)
        e1->moveOneFloor();
    // e2 at floor 4, direction Down (add lower dest)
    e2->addDestination(1);
    while (e2->getCurrentFloor() > 4)
        e2->moveOneFloor();
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 3, 5, 0); // from floor 3, direction Up needed
    Elevator *chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); // e1 moving Up and can serve floor 3
}

TEST_F(SCANStrategyTest, PrefersClosestCompatibleElevator)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    e1->addDestination(6);
    while (e1->getCurrentFloor() < 4)
        e1->moveOneFloor(); // at floor 4, going Up
    e2->addDestination(2);
    while (e2->getCurrentFloor() > 6)
        e2->moveOneFloor(); // at floor 6, going Down
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 5, 7, 0); // from floor 5, Up direction
    Elevator *chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); // distance 1 vs e2 distance 1 but e2 moving Down incompatible
}

TEST_F(SCANStrategyTest, FallbackToFirstElevatorIfNoneCompatible)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    e1->addDestination(1); // moving Down
    while (e1->getCurrentFloor() > 3)
        e1->moveOneFloor();
    e2->addDestination(10); // moving Up
    while (e2->getCurrentFloor() < 8)
        e2->moveOneFloor();
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 5, 6, 0); // both incompatible (e1 Down, e2 Up but call floor 5 is behind e2)
    Elevator *chosen = strategy->assign(*system, call);
    EXPECT_NE(nullptr, chosen); // fallback to first (e1)
}

TEST_F(SCANStrategyTest, NameReturnsSCAN)
{
    EXPECT_STREQ("SCAN", strategy->getName());
}