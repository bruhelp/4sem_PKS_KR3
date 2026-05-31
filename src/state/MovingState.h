#pragma once

#include "IElevatorState.h"

class MovingState : public IElevatorState
{
public:

    void handle(Elevator& elevator) override;

    const char* getName() const override;
};