#include "SCANStrategy.h"

#include "../core/ElevatorSystem.h"
#include "../core/Elevator.h"
#include "../model/Call.h"
#include "../model/Direction.h"

#include <cmath>

Elevator* SCANStrategy::assign(
    ElevatorSystem& system,
    const Call& call)
{
    auto& elevators =
        system.getElevators();

    Elevator* best = nullptr;

    int score =
        std::numeric_limits<int>::max();

    for (auto& elevator : elevators)
    {
        int currentFloor =
            elevator->getCurrentFloor();

        Direction direction =
            elevator->getDirection();

        int currentScore =
            std::abs(
                currentFloor
                -
                call.getFromFloor());

        bool compatible =
            false;

        if (
            direction == Direction::Idle
        )
        {
            compatible = true;
        }
        else if (
            direction == Direction::Up
            &&
            call.getFromFloor()
            >= currentFloor
        )
        {
            compatible = true;
        }
        else if (
            direction == Direction::Down
            &&
            call.getFromFloor()
            <= currentFloor
        )
        {
            compatible = true;
        }

        if (
            compatible
            &&
            currentScore < score
        )
        {
            score = currentScore;
            best = elevator.get();
        }
    }

    if (best == nullptr)
    {
        best = elevators.front().get();
    }

    return best;
}

const char* SCANStrategy::getName() const
{
    return "SCAN";
}