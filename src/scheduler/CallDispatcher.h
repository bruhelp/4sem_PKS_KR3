#pragma once

#include "IStrategy.h"
#include <memory>

class ElevatorSystem;
class Call;
class Elevator;

class IStrategy;

class CallDispatcher
{
private:

    std::unique_ptr<IStrategy> strategy;

public:

    CallDispatcher();

    void setStrategy(
        std::unique_ptr<IStrategy> strategy);

    Elevator* dispatchCall(
        ElevatorSystem& system,
        const Call& call);

    const char* getStrategyName() const;
};