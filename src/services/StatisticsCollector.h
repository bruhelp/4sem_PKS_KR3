#pragma once

#include <vector>

#include "../result/SimulationResult.h"
#include "Config.h"

class ElevatorSystem;

class StatisticsCollector
{
private:

    std::vector<SimulationResult> results;

public:

    SimulationResult collect(
        ElevatorSystem& system,
        const Config& config,
        double executionTime,
        const std::string& algorithmName);

    void saveResult(
        const SimulationResult& result);

    const std::vector<SimulationResult>&
    getResults() const;
};