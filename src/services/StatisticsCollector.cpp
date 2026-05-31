#include "StatisticsCollector.h"

#include "../core/ElevatorSystem.h"
#include "../core/Elevator.h"

#include <fstream>
#include <iomanip>
#include <algorithm>

SimulationResult
StatisticsCollector::collect(
    ElevatorSystem &system,
    int simulationDuration,
    double executionTime,
    const std::string &algorithmName)
{
    SimulationResult result;

    result.algorithmName =
        algorithmName;

    result.simulationDuration =
        simulationDuration;

    result.executionTime =
        executionTime;

    const auto &passengers =
        system.getCompletedPassengers();

    result.totalPassengers =
        static_cast<int>(
            passengers.size());

    double totalWait = 0.0;

    double totalRide = 0.0;

    double maxWait = 0.0;

    for (
        const auto &
            passenger :
        passengers)
    {
        totalWait +=
            passenger.getWaitingTime();

        totalRide +=
            passenger.getRideTime();

        maxWait =
            std::max(
                maxWait,
                static_cast<double>(
                    passenger
                        .getWaitingTime()));
    }

    if (!passengers.empty())
    {
        result.averageWaitTime =
            totalWait /
            passengers.size();

        result.averageRideTime =
            totalRide /
            passengers.size();

        result.maximumWaitTime =
            maxWait;
    }

    long long movementTime = 0;

    for (
        const auto &
            elevator :
        system.getElevators())
    {
        movementTime +=
            elevator
                ->getMovementTime();
    }

    if (
        !system.getElevators()
             .empty())
    {
        result.utilization =
            static_cast<double>(
                movementTime) /
            (simulationDuration * 1000.0 *
             system.getElevators()
                 .size());
    }

    results.push_back(
        result);

    return result;
}

void StatisticsCollector::saveResult(
    const SimulationResult &result)
{
    std::ofstream file(
        "results/simulation_history.txt",
        std::ios::app);

    if (!file.is_open())
    {
        return;
    }

    file
        << "==================================\n";

    file
        << "Algorithm: "
        << result.algorithmName
        << "\n";

    file
        << "Simulation duration: "
        << result.simulationDuration
        << " sec\n";

    file
        << "Execution time: "
        << result.executionTime
        << " sec\n";

    file
        << "Utilization: "
        << std::fixed
        << std::setprecision(2)
        << result.utilization * 100
        << "%\n";

    file
        << "Passengers: "
        << result.totalPassengers
        << "\n";

    file
        << "Average wait time: "
        << result.averageWaitTime
        << "\n";

    file
        << "Maximum wait time: "
        << result.maximumWaitTime
        << "\n";

    file
        << "Average ride time: "
        << result.averageRideTime
        << "\n";
        
    file
        << "==================================\n\n";
}

const std::vector<SimulationResult> &
StatisticsCollector::getResults() const
{
    return results;
}