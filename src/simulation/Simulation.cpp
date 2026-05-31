#include "Simulation.h"

#include <chrono>
#include <thread>
#include <random>
#include <iostream>

#include "../core/Elevator.h"

#include "../model/Passenger.h"

#include "../scheduler/FIFOStrategy.h"
#include "../scheduler/NearestCarStrategy.h"
#include "../scheduler/RoundRobinStrategy.h"
#include "../scheduler/SCANStrategy.h"

Simulation::Simulation(
    const Config &config)
    : config(config),
      system(&dispatcher, &logger),
      running(false)
{
}

void Simulation::run()
{
    logger.log(
        "Simulation started");

    switch (config.algorithm)
    {
    case AlgorithmType::FIFO:

        dispatcher.setStrategy(
            std::make_unique<
                FIFOStrategy>());

        break;

    case AlgorithmType::NEAREST_CAR:

        dispatcher.setStrategy(
            std::make_unique<
                NearestCarStrategy>());

        break;

    case AlgorithmType::ROUND_ROBIN:

        dispatcher.setStrategy(
            std::make_unique<
                RoundRobinStrategy>());

        break;

    case AlgorithmType::SCAN:

        dispatcher.setStrategy(
            std::make_unique<
                SCANStrategy>());

        break;
    }

    for (
        int i = 0;
        i < config.elevators;
        ++i)
    {
        system.addElevator(
            std::make_unique<Elevator>(
                i + 1,
                config.capacity,
                config.travelTimePerFloor,
                &logger));
    }

    system.startElevators();

    std::mt19937 generator(
        config.seed);

    std::uniform_int_distribution<>
        floorGenerator(
            1,
            config.floors);

    auto startTime =
        std::chrono::steady_clock::now();

    running = true;

    int currentTime = 0;

    while (
        running &&
        currentTime <
            config.simulationDuration)
    {
        int fromFloor =
            floorGenerator(
                generator);

        int toFloor =
            floorGenerator(
                generator);

        while (
            toFloor ==
            fromFloor)
        {
            toFloor =
                floorGenerator(
                    generator);
        }

        static int passengerId = 1;

        Passenger passenger(
            passengerId++,
            fromFloor,
            toFloor,
            currentTime);

        system.addPassenger(
            passenger);

        Call call(
            passenger.getId(),
            fromFloor,
            toFloor,
            currentTime);

        system.addCall(call);

        system.processCalls();

        for (
            auto &elevator :
            system.getElevators())
        {
            system.processBoarding(
                *elevator,
                currentTime);
        }

        renderer.render(
            system,
            config.floors,
            currentTime,
            config.simulationDuration);

        std::this_thread::sleep_for(
            std::chrono::seconds(1));

        ++currentTime;
    }

    system.stopElevators();

    auto endTime =
        std::chrono::steady_clock::now();

    double executionTime =
        std::chrono::duration<double>(
            endTime -
            startTime)
            .count();

    SimulationResult result =
        statistics.collect(
            system,
            config.simulationDuration,
            executionTime,
            dispatcher.getStrategyName());

    statistics.saveResult(
        result);

    std::cout
        << "\nSimulation finished\n";

    std::cout
        << "Algorithm: "
        << result.algorithmName
        << "\n";

    std::cout
        << "Execution time: "
        << result.executionTime
        << " sec\n";

    std::cout
        << "Utilization: "
        << result.utilization * 100
        << "%\n";

    logger.log(
        "Simulation finished");
}

void Simulation::stop()
{
    running = false;
}