#pragma once
#include <vector>
#include "core/Elevator.h"
#include "model/Call.h"

class ElevatorSystem {
public:
    ElevatorSystem(int elevatorsCount, int floors);

    void step();
    void addCall(const Call& call);

private:
    std::vector<Elevator> elevators;
    std::vector<Call> calls;
};