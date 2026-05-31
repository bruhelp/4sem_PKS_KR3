#include "Config.h"

#include <iostream>

bool Config::isValid() const
{
    return
        floors >= 2 &&
        floors <= 50 &&

        elevators >= 1 &&
        elevators <= 6 &&

        capacity >= 1 &&
        capacity <= 20 &&

        passengersPerFloor >= 1 &&
        passengersPerFloor <= 50 &&

        simulationDuration > 0;
}

Config Config::readFromConsole()
{
    Config config;

    std::cout << "Floors (2-50): ";
    std::cin >> config.floors;

    std::cout << "Elevators (1-6): ";
    std::cin >> config.elevators;

    std::cout << "Capacity (1-20): ";
    std::cin >> config.capacity;

    std::cout << "Passengers per floor (1-50): ";
    std::cin >> config.passengersPerFloor;

    std::cout << "Travel time per floor (ms): ";
    std::cin >> config.travelTimePerFloor;

    std::cout << "Simulation duration (sec): ";
    std::cin >> config.simulationDuration;

    std::cout << "Random seed: ";
    std::cin >> config.seed;

    int alg;

    std::cout
        << "\n1 - FIFO\n"
        << "2 - Nearest Car\n"
        << "3 - Round Robin\n"
        << "4 - SCAN\n";

    std::cin >> alg;

    config.algorithm =
        static_cast<AlgorithmType>(alg);

    return config;
}