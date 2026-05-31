#pragma once

class ElevatorSystem;
class Call;
class Elevator;

class IStrategy
{
public:

    virtual ~IStrategy() = default;

    virtual Elevator* assign(
        ElevatorSystem& system,
        const Call& call) = 0;

    virtual const char* getName() const = 0;
};