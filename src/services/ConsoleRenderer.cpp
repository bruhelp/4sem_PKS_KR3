#include "ConsoleRenderer.h"

#include "../core/ElevatorSystem.h"
#include "../core/Elevator.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>

void ConsoleRenderer::clearScreen() const
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void ConsoleRenderer::render(
    const ElevatorSystem& elevatorSystem,
    int floors,
    int currentTime,
    int simulationDuration) const
{
    clearScreen();

    std::cout
        << "============================================================\n";

    std::cout
        << "                ELEVATOR SIMULATOR\n";

    std::cout
        << " Time: "
        << currentTime
        << " / "
        << simulationDuration
        << " sec\n";

    std::cout
        << " Pending calls: "
        << elevatorSystem.getPendingCallCount()
        << "\n";

    std::cout
        << "============================================================\n\n";

    const auto& elevators =
        elevatorSystem.getElevators();

    for (
        int floor = floors;
        floor >= 1;
        --floor
    )
    {
        std::cout
            << std::setw(3)
            << floor
            << " | ";

        for (const auto& elevator : elevators)
        {
            if (
                elevator->getCurrentFloor()
                ==
                floor
            )
            {
                if (elevator->isDoorOpen())
                {
                    std::cout
                        << "[| |]";
                }
                else
                {
                    std::cout
                        << "[|#|]";
                }
            }
            else
            {
                std::cout
                    << "[   ]";
            }

            std::cout << " ";
        }

        std::cout << '\n';
    }

    std::cout
        << "\n";

    std::cout
        << "============================================================\n";

    for (const auto& elevator : elevators)
    {
        std::cout
            << "Elevator #"
            << elevator->getId()
            << " | Floor "
            << elevator->getCurrentFloor()
            << " | Load "
            << elevator->getPassengerCount()
            << "/"
            << elevator->getCapacity()
            << " | State "
            << elevator->getStateName()
            << '\n';
    }

    std::cout
        << "============================================================\n";
}