#pragma once

#include "IElevatorState.h"

class StoppedState : public IElevatorState
{
public:

    void handle(Elevator& elevator) override;

    const char* getName() const override;
};