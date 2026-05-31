#include "services/Config.h"

#include "simulation/Simulation.h"

#include <iostream>

int main()
{
    Config config =
        Config::readFromConsole();

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