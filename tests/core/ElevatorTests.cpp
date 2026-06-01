#include <gtest/gtest.h>
#include "../src/core/Elevator.h"
#include "../src/model/Passenger.h"

// Helper to create an elevator without starting its thread
class ElevatorTest : public ::testing::Test
{
protected:
    Elevator *createElevator(int id = 1, int capacity = 8, int travelTime = 100)
    {
        // logger = nullptr to avoid file/console output
        return new Elevator(id, capacity, travelTime, nullptr);
    }
};

// ----- Constructor and basic getters -----
TEST_F(ElevatorTest, ConstructorSetsInitialState)
{
    auto *e = createElevator(5, 10, 50);
    EXPECT_EQ(5, e->getId());
    EXPECT_EQ(1, e->getCurrentFloor());
    EXPECT_EQ(0, e->getPassengerCount());
    EXPECT_EQ(10, e->getCapacity());
    EXPECT_EQ(Direction::Idle, e->getDirection());
    EXPECT_FALSE(e->isDoorOpen());
    EXPECT_EQ(0, e->getMovementTime());
    EXPECT_EQ(0, e->getServiceTime());
    EXPECT_TRUE(e->hasFreeSpace());
    EXPECT_EQ("Idle", e->getStateName());
    delete e;
}

// ----- addDestination / hasDestinations / getPendingStopsCount -----
TEST_F(ElevatorTest, AddDestinationIncreasesPendingStops)
{
    auto *e = createElevator();
    EXPECT_FALSE(e->hasDestinations());
    EXPECT_EQ(0, e->getPendingStopsCount());

    e->addDestination(5);
    EXPECT_TRUE(e->hasDestinations());
    EXPECT_EQ(1, e->getPendingStopsCount());

    e->addDestination(3);
    EXPECT_EQ(2, e->getPendingStopsCount());

    delete e;
}

TEST_F(ElevatorTest, GetDestinationListReturnsOrderedDestinations)
{
    auto *e = createElevator();
    e->addDestination(10);
    e->addDestination(7);
    e->addDestination(3);
    std::vector<int> expected = {10, 7, 3};
    EXPECT_EQ(expected, e->getDestinationList());
    delete e;
}

TEST_F(ElevatorTest, GetCurrentTargetReturnsFrontDestination)
{
    auto *e = createElevator();
    EXPECT_EQ(-1, e->getCurrentTarget());
    e->addDestination(8);
    EXPECT_EQ(8, e->getCurrentTarget());
    e->addDestination(2);
    EXPECT_EQ(8, e->getCurrentTarget()); // still front
    delete e;
}

// ----- moveOneFloor -----
TEST_F(ElevatorTest, MoveOneFloorUpWhenDestinationHigher)
{
    auto *e = createElevator();
    e->addDestination(3);
    EXPECT_EQ(1, e->getCurrentFloor());
    e->moveOneFloor();
    EXPECT_EQ(2, e->getCurrentFloor());
    EXPECT_EQ(Direction::Up, e->getDirection());
    EXPECT_GT(e->getMovementTime(), 0);
}

TEST_F(ElevatorTest, MoveOneFloorDownWhenDestinationLower)
{
    auto *e = createElevator();
    e->addDestination(0); // floor 0 (valid for test)
    e->moveOneFloor();
    EXPECT_EQ(0, e->getCurrentFloor());
    EXPECT_EQ(Direction::Down, e->getDirection());
}

TEST_F(ElevatorTest, MoveOneFloorDoesNothingIfNoDestinations)
{
    auto *e = createElevator();
    int initialFloor = e->getCurrentFloor();
    e->moveOneFloor();
    EXPECT_EQ(initialFloor, e->getCurrentFloor());
    EXPECT_EQ(Direction::Idle, e->getDirection());
}

TEST_F(ElevatorTest, MoveOneFloorStopsAtTargetAndDirectionBecomesIdleAfterPop)
{
    auto *e = createElevator();
    e->addDestination(2);
    e->moveOneFloor(); // now floor 2, target reached
    EXPECT_EQ(2, e->getCurrentFloor());
    EXPECT_TRUE(e->isOnTargetFloor());
    // direction stays Up until next move or stop handling
    EXPECT_EQ(Direction::Up, e->getDirection());
    delete e;
}

// ----- isOnTargetFloor -----
TEST_F(ElevatorTest, IsOnTargetFloorTrueWhenCurrentEqualsFront)
{
    auto *e = createElevator();
    e->addDestination(5);
    e->addDestination(1);
    // manually set current floor to 5 (simulate movement)
    // but we cannot set private currentFloor; use moveOneFloor repeatedly
    while (e->getCurrentFloor() < 5)
        e->moveOneFloor();
    EXPECT_TRUE(e->isOnTargetFloor());
}

TEST_F(ElevatorTest, IsOnTargetFloorFalseWhenNoDestinations)
{
    auto *e = createElevator();
    EXPECT_FALSE(e->isOnTargetFloor());
}

TEST_F(ElevatorTest, HandleStopOpensClosesDoorsAndUnloadsMatchingPassengers)
{
    auto *e = createElevator(1, 2, 100);
    Passenger p1(1, 2, 2, 0);
    Passenger p2(2, 2, 3, 0);
    e->boardPassenger(p1);
    e->boardPassenger(p2);
    while (e->getCurrentFloor() < 2)
        e->moveOneFloor();
    EXPECT_TRUE(e->isOnTargetFloor());
    e->handleStop();
    EXPECT_FALSE(e->isDoorOpen());
    EXPECT_EQ(1, e->getPassengerCount()); 

    EXPECT_EQ(1, e->getPendingStopsCount());
    EXPECT_EQ(3, e->getCurrentTarget());
}

TEST_F(ElevatorTest, BoardPassengerAddsToPassengersAndDestination)
{
    auto *e = createElevator(1, 2, 100);
    Passenger p(10, 1, 5, 0);
    e->boardPassenger(p);
    EXPECT_EQ(1, e->getPassengerCount());
    EXPECT_TRUE(e->hasDestinations());
    EXPECT_EQ(5, e->getCurrentTarget());
}

TEST_F(ElevatorTest, BoardPassengerRespectsCapacity)
{
    auto *e = createElevator(1, 1, 100);
    Passenger p1(1, 1, 2, 0);
    Passenger p2(2, 1, 3, 0);
    e->boardPassenger(p1);
    e->boardPassenger(p2);
    EXPECT_EQ(1, e->getPassengerCount());
    EXPECT_FALSE(e->hasFreeSpace());
}

TEST_F(ElevatorTest, UnloadPassengersWithSimulationTimeSetsArrivalTime)
{
    auto *e = createElevator();
    Passenger p(100, 3, 3, 10);
    e->boardPassenger(p);
    // move to floor 3
    while (e->getCurrentFloor() < 3)
        e->moveOneFloor();
    auto unloaded = e->unloadPassengers(1000);
    ASSERT_EQ(1, unloaded.size());
    EXPECT_EQ(1000, unloaded[0].getArrivalTime());
    EXPECT_EQ(0, e->getPassengerCount());
}

TEST_F(ElevatorTest, ServiceTimeAccumulatesDuringHandleStop)
{
    auto *e = createElevator(1, 1, 100);
    e->addDestination(2);
    while (e->getCurrentFloor() < 2)
        e->moveOneFloor();
    long long before = e->getServiceTime();
    e->handleStop();
    EXPECT_GT(e->getServiceTime(), before);
}