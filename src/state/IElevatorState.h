#pragma once

class Elevator;

class IElevatorState
{
public:

    virtual ~IElevatorState() = default;

    virtual void handle(Elevator& elevator) = 0;

    virtual const char* getName() const = 0;
};