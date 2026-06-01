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

    e1->addDestination(3);
    while (e1->getCurrentFloor() < 2)
        e1->moveOneFloor();

    e2->addDestination(1);
    while (e2->getCurrentFloor() > 4)
        e2->moveOneFloor();
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 3, 5, 0); 
    Elevator *chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); 
}

TEST_F(SCANStrategyTest, PrefersClosestCompatibleElevator)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    e1->addDestination(6);
    while (e1->getCurrentFloor() < 4)
        e1->moveOneFloor(); 
    e2->addDestination(2);
    while (e2->getCurrentFloor() > 6)
        e2->moveOneFloor(); 
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 5, 7, 0);
    Elevator *chosen = strategy->assign(*system, call);
    EXPECT_EQ(1, chosen->getId()); 
}

TEST_F(SCANStrategyTest, FallbackToFirstElevatorIfNoneCompatible)
{
    auto e1 = std::make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = std::make_unique<Elevator>(2, 5, 100, nullptr);
    e1->addDestination(1); // moving Down
    while (e1->getCurrentFloor() > 3)
        e1->moveOneFloor();
    e2->addDestination(10);
    while (e2->getCurrentFloor() < 8)
        e2->moveOneFloor();
    system->addElevator(std::move(e1));
    system->addElevator(std::move(e2));
    Call call(1, 5, 6, 0); 
    Elevator *chosen = strategy->assign(*system, call);
    EXPECT_NE(nullptr, chosen); 
}

TEST_F(SCANStrategyTest, NameReturnsSCAN)
{
    EXPECT_STREQ("SCAN", strategy->getName());
}