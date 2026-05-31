#include "FIFOStrategy.h"

#include "../core/ElevatorSystem.h"
#include "../core/Elevator.h"
#include "../model/Call.h"

Elevator* FIFOStrategy::assign(
    ElevatorSystem& system,
    const Call&)
{
    auto& elevators =
        system.getElevators();

    Elevator* best = nullptr;

    int minQueue =
        std::numeric_limits<int>::max();

    for (auto& elevator : elevators)
    {
        int queueSize =
            elevator->getPendingStopsCount();

        if (queueSize < minQueue)
        {
            minQueue = queueSize;
            best = elevator.get();
        }
    }

    return best;
}

const char* FIFOStrategy::getName() const
{
    return "FIFO";
}