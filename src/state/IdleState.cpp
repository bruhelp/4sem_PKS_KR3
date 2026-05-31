#include "IdleState.h"

#include "../core/Elevator.h"
#include "MovingState.h"

void IdleState::handle(Elevator& elevator)
{
    if (elevator.hasDestinations())
    {
        elevator.setState(
            std::make_unique<MovingState>());
    }
}

const char* IdleState::getName() const
{
    return "Idle";
}