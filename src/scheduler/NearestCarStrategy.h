#pragma once

#include "IStrategy.h"

class NearestCarStrategy : public IStrategy
{
public:

    Elevator* assign(
        ElevatorSystem& system,
        const Call& call) override;

    const char* getName() const override;
};