#pragma once
#include "state/IElevatorState.h"

class IdleState : public IElevatorState {
public:
    void handle(Elevator& e) override;
};