#include "services/Config.h"
#include "simulation/Simulation.h"

#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
    Config config;

    if (argc == 6)
    {
        config.floors = std::atoi(argv[1]);
        config.elevators = std::atoi(argv[2]);
        config.capacity = std::atoi(argv[3]);
        config.passengersPerFloor = std::atoi(argv[4]);
        config.simulationDuration = std::atoi(argv[5]);

        std::cout
            << "Configuration loaded from command line.\n";
    }
    
    else
    {
        config =
            Config::readFromConsole();
    }

    if (!config.isValid())
    {
        std::cout
            << "Invalid configuration\n";

        return 1;
    }

    Simulation simulation(
        config);

    simulation.run();

    return 0;
}