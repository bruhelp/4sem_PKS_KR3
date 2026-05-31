#pragma once

#include "../services/Config.h"
#include "../services/StatisticsCollector.h"
#include "../services/EventLogger.h"
#include "../services/ConsoleRenderer.h"
#include "../scheduler/CallDispatcher.h"
#include "../core/ElevatorSystem.h"

class Simulation
{
private:

    Config config;

    EventLogger logger;

    StatisticsCollector statistics;

    ConsoleRenderer renderer;

    CallDispatcher dispatcher;

    ElevatorSystem system;

    bool running;

public:

    explicit Simulation(
        const Config& config);

    void run();

    void stop();
};