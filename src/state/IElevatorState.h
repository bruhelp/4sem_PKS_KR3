#pragma once

class Elevator;

class IElevatorState {
public:
    virtual void handle(Elevator& e) = 0;
    virtual ~IElevatorState() = default;
};