#include <iostream>
#include "simulation/Simulation.h"
#include "core/ElevatorSystem.h"

void Simulation::run() {
    std::cout << "Simulation started\n";

    ElevatorSystem system(2, 10); // 2 лифта, 10 этажей

    for (int t = 0; t < 10; ++t) {
        std::cout << "\n--- Time step " << t << " ---\n";
        system.step();
    }

    std::cout << "Simulation finished\n";
}