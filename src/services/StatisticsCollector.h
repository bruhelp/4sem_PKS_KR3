#pragma once

#include <vector>

#include "../result/SimulationResult.h"

class ElevatorSystem;

class StatisticsCollector
{
private:

    std::vector<SimulationResult> results;

public:

    SimulationResult collect(
        ElevatorSystem& system,
        int simulationDuration,
        double executionTime,
        const std::string& algorithmName);

    void saveResult(
        const SimulationResult& result);

    const std::vector<SimulationResult>&
    getResults() const;
};