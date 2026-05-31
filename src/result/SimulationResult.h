#pragma once

#include <string>

struct SimulationResult
{
    std::string algorithmName;

    double averageWaitTime = 0.0;

    double maximumWaitTime = 0.0;

    double averageRideTime = 0.0;

    double utilization = 0.0;

    int totalPassengers = 0;

    double totalDoorTime = 0.0;

    double totalBoardingTime = 0.0;

    int simulationDuration = 0;

    double executionTime = 0.0;
};