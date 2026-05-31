#include "MovingState.h"

#include "../core/Elevator.h"
#include "StoppedState.h"

void MovingState::handle(Elevator& elevator)
{
    elevator.moveOneFloor();

    if (elevator.isOnTargetFloor())
    {
        elevator.setState(
            std::make_unique<StoppedState>());
    }
}

const char* MovingState::getName() const
{
    return "Moving";
}