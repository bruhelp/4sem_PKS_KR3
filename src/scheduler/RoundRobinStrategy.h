#pragma once

#include "IStrategy.h"
#include <cstddef>

class RoundRobinStrategy : public IStrategy
{
private:

    size_t currentIndex;

public:

    RoundRobinStrategy();

    Elevator* assign(
        ElevatorSystem& system,
        const Call& call) override;

    const char* getName() const override;
};