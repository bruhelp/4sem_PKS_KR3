#include <gtest/gtest.h>
#include "../src/scheduler/RoundRobinStrategy.h"
#include "../src/core/ElevatorSystem.h"
#include "../src/core/Elevator.h"
#include "../src/model/Call.h"

class RoundRobinStrategyTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        system = new ElevatorSystem(nullptr, nullptr);
        strategy = std::make_unique<RoundRobinStrategy>();
    }

    void TearDown() override
    {
        delete system;
    }

    std::unique_ptr<RoundRobinStrategy> strategy;
    ElevatorSystem* system;
};

TEST_F(RoundRobinStrategyTest, AssignsElevatorsInCyclicOrder)
{
    system->addElevator(std::make_unique<Elevator>(1, 5, 100, nullptr));
    system->addElevator(std::make_unique<Elevator>(2, 5, 100, nullptr));
    system->addElevator(std::make_unique<Elevator>(3, 5, 100, nullptr));

    Call dummy(1, 1, 2, 0);
    Elevator* first = strategy->assign(*system, dummy);
    Elevator* second = strategy->assign(*system, dummy);
    Elevator* third = strategy->assign(*system, dummy);
    Elevator* fourth = strategy->assign(*system, dummy);

    EXPECT_EQ(1, first->getId());
    EXPECT_EQ(2, second->getId());
    EXPECT_EQ(3, third->getId());
    EXPECT_EQ(1, fourth->getId());
}

TEST_F(RoundRobinStrategyTest, WorksWithSingleElevator)
{
    system->addElevator(std::make_unique<Elevator>(42, 5, 100, nullptr));
    Call call(1, 1, 2, 0);
    for (int i = 0; i < 5; ++i)
    {
        Elevator* chosen = strategy->assign(*system, call);
        EXPECT_EQ(42, chosen->getId());
    }
}

TEST_F(RoundRobinStrategyTest, IgnoresCallParameters)
{
    system->addElevator(std::make_unique<Elevator>(1, 5, 100, nullptr));
    system->addElevator(std::make_unique<Elevator>(2, 5, 100, nullptr));
    Call call1(1, 100, 200, 0);
    Call call2(2, -5, -10, 0);
    Elevator* e1 = strategy->assign(*system, call1);
    Elevator* e2 = strategy->assign(*system, call2);
    EXPECT_EQ(1, e1->getId());
    EXPECT_EQ(2, e2->getId());
}

TEST_F(RoundRobinStrategyTest, ReturnsNullptrIfNoElevators)
{
    ElevatorSystem emptySys(nullptr, nullptr);
    Call call(1, 1, 2, 0);
    Elevator* chosen = strategy->assign(emptySys, call);
    EXPECT_EQ(nullptr, chosen);
}

TEST_F(RoundRobinStrategyTest, NameReturnsRoundRobin)
{
    EXPECT_STREQ("Round Robin", strategy->getName());
}