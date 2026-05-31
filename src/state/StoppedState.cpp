#include "StoppedState.h"

#include "../core/Elevator.h"

#include "IdleState.h"
#include "MovingState.h"

void StoppedState::handle(Elevator& elevator)
{
    elevator.handleStop();

    if (elevator.hasDestinations())
    {
        elevator.setState(
            std::make_unique<MovingState>());
    }
    else
    {
        elevator.setState(
            std::make_unique<IdleState>());
    }
}

const char* StoppedState::getName() const
{
    return "Stopped";
}