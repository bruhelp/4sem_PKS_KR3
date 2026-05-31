#include "CallDispatcher.h"

#include "IStrategy.h"

#include "../core/ElevatorSystem.h"

CallDispatcher::CallDispatcher()
{
}

void CallDispatcher::setStrategy(
    std::unique_ptr<IStrategy> strategy)
{
    this->strategy =
        std::move(strategy);
}

Elevator* CallDispatcher::dispatchCall(
    ElevatorSystem& system,
    const Call& call)
{
    if (!strategy)
    {
        return nullptr;
    }

    return strategy->assign(
        system,
        call);
}

const char* CallDispatcher::getStrategyName() const
{
    if (!strategy)
    {
        return "Unknown";
    }

    return strategy->getName();
}