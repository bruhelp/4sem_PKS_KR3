#pragma once

#include <string>

struct SimulationResult
{
    double averageWaitTime = 0.0;

    double maximumWaitTime = 0.0;

    double averageRideTime = 0.0;

    double utilization = 0.0;

    int totalPassengers = 0;

    double totalDoorTime = 0.0;

    double totalBoardingTime = 0.0;

    int simulationDuration = 0;

    double executionTime = 0.0;

    long long totalMovementTime = 0;

    long long totalServiceTime = 0;

    int floors = 0;

    int elevators = 0;

    int capacity = 0;

    int passengersPerFloor = 0;

    int travelTimePerFloor = 0;

    int seed = 0;

    std::string algorithmName;
};