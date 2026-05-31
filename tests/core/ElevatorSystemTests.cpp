#include <gtest/gtest.h>
#include "../src/core/ElevatorSystem.h"
#include "../src/core/Elevator.h"
#include "../src/scheduler/NearestCarStrategy.h"
#include "../src/model/Call.h"
#include "../src/model/Passenger.h"
#include "../../src/scheduler/CallDispatcher.h"

class ElevatorSystemTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        dispatcher.setStrategy(std::make_unique<NearestCarStrategy>());
        system = new ElevatorSystem(&dispatcher, nullptr);
    }

    void TearDown() override
    {
        delete system;
    }

    CallDispatcher dispatcher;
    ElevatorSystem* system;
};

TEST_F(ElevatorSystemTest, AddElevatorIncreasesElevatorCount)
{
    EXPECT_EQ(0, system->getElevators().size());
    system->addElevator(std::make_unique<Elevator>(1, 5, 100, nullptr));
    EXPECT_EQ(1, system->getElevators().size());
}

TEST_F(ElevatorSystemTest, GetElevatorsReturnsCorrectReference)
{
    auto& elevators = system->getElevators();
    EXPECT_TRUE(elevators.empty());
    system->addElevator(std::make_unique<Elevator>(1, 5, 100, nullptr));
    system->addElevator(std::make_unique<Elevator>(2, 8, 100, nullptr));
    EXPECT_EQ(2, elevators.size());
    EXPECT_EQ(1, elevators[0]->getId());
    EXPECT_EQ(2, elevators[1]->getId());
}

TEST_F(ElevatorSystemTest, AddCallIncreasesPendingCalls)
{
    EXPECT_EQ(0, system->getPendingCallCount());
    Call call(1, 1, 3, 0);
    system->addCall(call);
    EXPECT_EQ(1, system->getPendingCallCount());
}

TEST_F(ElevatorSystemTest, ProcessCallsAssignsDestinationsToElevator)
{
    system->addElevator(std::make_unique<Elevator>(1, 5, 100, nullptr));
    system->startElevators(); // threads needed for processing? processCalls is synchronous.
    Call call(1, 2, 5, 0);
    system->addCall(call);
    system->processCalls();
    auto& elev = system->getElevators()[0];
    EXPECT_TRUE(elev->hasDestinations());
    auto destList = elev->getDestinationList();
    ASSERT_EQ(2, destList.size());
    EXPECT_EQ(2, destList[0]); // from floor
    EXPECT_EQ(5, destList[1]); // to floor
    system->stopElevators();
}

TEST_F(ElevatorSystemTest, AddPassengerAndGetWaitingCount)
{
    EXPECT_EQ(0, system->getWaitingCount(1));
    Passenger p(10, 1, 3, 0);
    system->addPassenger(p);
    EXPECT_EQ(1, system->getWaitingCount(1));
    EXPECT_EQ(0, system->getWaitingCount(2));
}

TEST_F(ElevatorSystemTest, ProcessBoardingLoadsWaitingPassengers)
{
    system->addElevator(std::make_unique<Elevator>(1, 2, 100, nullptr));
    auto& elev = system->getElevators()[0];
    // add waiting passenger on floor 1
    Passenger p(99, 1, 4, 0);
    system->addPassenger(p);
    // elevator starts at floor 1
    system->processBoarding(*elev, 100);
    EXPECT_EQ(1, elev->getPassengerCount());
    EXPECT_EQ(0, system->getWaitingCount(1));
}

TEST_F(ElevatorSystemTest, ProcessBoardingUnloadsAtDestination)
{
    system->addElevator(std::make_unique<Elevator>(1, 2, 100, nullptr));
    auto& elev = system->getElevators()[0];
    Passenger p(88, 1, 1, 0); // start and dest same floor
    elev->boardPassenger(p);
    // move to floor 1 (already there)
    system->processBoarding(*elev, 200);
    const auto& completed = system->getCompletedPassengers();
    ASSERT_EQ(1, completed.size());
    EXPECT_EQ(88, completed[0].getId());
    EXPECT_EQ(200, completed[0].getArrivalTime());
    EXPECT_EQ(0, elev->getPassengerCount());
}