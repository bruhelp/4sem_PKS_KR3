#include "RoundRobinStrategy.h"

#include "../core/ElevatorSystem.h"

RoundRobinStrategy::RoundRobinStrategy()
    : currentIndex(0)
{
}

Elevator *RoundRobinStrategy::assign(
    ElevatorSystem &system,
    const Call &call)
{
    auto &elevators =
        system.getElevators();

    if (elevators.empty())
        return nullptr;

    Elevator *result =
        elevators[currentIndex].get();

    currentIndex =
        (currentIndex + 1) %
        elevators.size();

    return result;
}

const char *RoundRobinStrategy::getName() const
{
    return "Round Robin";
}