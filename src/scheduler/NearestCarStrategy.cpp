#include "NearestCarStrategy.h"

#include "../core/ElevatorSystem.h"
#include "../core/Elevator.h"
#include "../model/Call.h"

#include <cmath>

Elevator* NearestCarStrategy::assign(
    ElevatorSystem& system,
    const Call& call)
{
    auto& elevators =
        system.getElevators();

    Elevator* best = nullptr;

    int bestDistance =
        std::numeric_limits<int>::max();

    for (auto& elevator : elevators)
    {
        int distance =
            std::abs(
                elevator->getCurrentFloor()
                -
                call.getFromFloor());

        if (distance < bestDistance)
        {
            bestDistance = distance;
            best = elevator.get();
        }
    }

    return best;
}

const char* NearestCarStrategy::getName() const
{
    return "Nearest Car";
}