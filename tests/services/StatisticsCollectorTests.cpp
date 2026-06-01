#include <gtest/gtest.h>
#include "../src/services/StatisticsCollector.h"
#include "../src/services/Config.h"
#include "../src/core/ElevatorSystem.h"
#include "../src/scheduler/FIFOStrategy.h"
#include "../src/model/Passenger.h"
#include "../../src/scheduler/CallDispatcher.h"
#include "../../src/core/Elevator.h"

class StatisticsCollectorTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        config.floors = 10;
        config.elevators = 2;
        config.capacity = 8;
        config.passengersPerFloor = 5;
        config.travelTimePerFloor = 100;
        config.simulationDuration = 60;
        config.seed = 123;
        config.algorithm = AlgorithmType::FIFO;
        dispatcher.setStrategy(std::make_unique<FIFOStrategy>());
        system = new ElevatorSystem(&dispatcher, nullptr);
        // add elevators
        for (int i = 1; i <= config.elevators; ++i)
            system->addElevator(std::make_unique<Elevator>(i, config.capacity, config.travelTimePerFloor, nullptr));
    }

    void TearDown() override
    {
        delete system;
    }

    Config config;
    CallDispatcher dispatcher;
    ElevatorSystem *system;
    StatisticsCollector stats;
};

TEST_F(StatisticsCollectorTest, CollectReturnsZeroAveragesWhenNoPassengers)
{
    SimulationResult result = stats.collect(*system, config, 1.5, "FIFO");
    EXPECT_EQ(0, result.totalPassengers);
    EXPECT_DOUBLE_EQ(0.0, result.averageWaitTime);
    EXPECT_DOUBLE_EQ(0.0, result.averageRideTime);
    EXPECT_DOUBLE_EQ(0.0, result.maximumWaitTime);
}

TEST_F(StatisticsCollectorTest, CollectComputesAverageWaitTimeCorrectly)
{
    Passenger p1(1, 1, 2, 0);
    Passenger p2(2, 1, 3, 5);
    p1.setPickupTime(10);
    p2.setPickupTime(8);
    system->addPassenger(p1);
    system->addPassenger(p2);
    Passenger p1_fixed(1, 1, 2, 0);
    Passenger p2_fixed(2, 1, 2, 5);
    p1_fixed.setPickupTime(10);
    p2_fixed.setPickupTime(8);

    auto &e = system->getElevators()[0];
    e->boardPassenger(p1_fixed);
    e->boardPassenger(p2_fixed);

    e->addDestination(2);
    while (e->getCurrentFloor() < 2)
        e->moveOneFloor();

    system->processBoarding(*e, 20); // 20 - время прибытия

    SimulationResult result = stats.collect(*system, config, 1.0, "FIFO");
    EXPECT_DOUBLE_EQ(6.5, result.averageWaitTime);
    EXPECT_EQ(2, result.totalPassengers);
}

TEST_F(StatisticsCollectorTest, CollectComputesMaxWaitTime)
{
    Passenger p1(1, 1, 2, 0);
    Passenger p2(2, 1, 2, 100);
    p1.setPickupTime(50);
    p2.setPickupTime(101);
    
    auto& e = system->getElevators()[0];
    e->boardPassenger(p1);
    e->boardPassenger(p2);
    e->addDestination(2);
    while (e->getCurrentFloor() < 2) e->moveOneFloor();
    system->processBoarding(*e, 150);
    
    SimulationResult result = stats.collect(*system, config, 1.0, "FIFO");
    EXPECT_DOUBLE_EQ(50, result.maximumWaitTime); 
}

TEST_F(StatisticsCollectorTest, CollectComputesUtilization)
{
    // move elevators manually
    auto &e1 = system->getElevators()[0];
    e1->addDestination(3);
    while (e1->getCurrentFloor() < 3)
        e1->moveOneFloor();
    long long movement = e1->getMovementTime();
    SimulationResult result = stats.collect(*system, config, 2.0, "FIFO");
    double expectedUtil = movement / (60.0 * 1000 * 2); // 2 elevators
    EXPECT_NEAR(expectedUtil, result.utilization, 0.0001);
}

TEST_F(StatisticsCollectorTest, CollectCopiesConfigFields)
{
    SimulationResult result = stats.collect(*system, config, 2.5, "TestAlgo");
    EXPECT_EQ(10, result.floors);
    EXPECT_EQ(2, result.elevators);
    EXPECT_EQ(8, result.capacity);
    EXPECT_EQ(5, result.passengersPerFloor);
    EXPECT_EQ(100, result.travelTimePerFloor);
    EXPECT_EQ(60, result.simulationDuration);
    EXPECT_EQ(123U, result.seed);
    EXPECT_EQ("TestAlgo", result.algorithmName);
}

TEST_F(StatisticsCollectorTest, SaveResultDoesNotCrash)
{
    SimulationResult result;
    result.algorithmName = "FIFO";
    EXPECT_NO_THROW(stats.saveResult(result));
}

TEST_F(StatisticsCollectorTest, GetResultsReturnsStoredResults)
{
    SimulationResult r1 = stats.collect(*system, config, 1.0, "Algo1");
    SimulationResult r2 = stats.collect(*system, config, 2.0, "Algo2");
    
    const auto& results = stats.getResults();
    ASSERT_EQ(2, results.size());
    EXPECT_EQ("Algo1", results[0].algorithmName);
    EXPECT_EQ("Algo2", results[1].algorithmName);
}