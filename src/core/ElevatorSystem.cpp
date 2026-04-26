#include <iostream>
#include "core/ElevatorSystem.h"

ElevatorSystem::ElevatorSystem(int count, int) {
    for (int i = 0; i < count; i++) {
        elevators.emplace_back(i);
    }

    // начальные вызовы
    calls.push_back({0, 5});
    calls.push_back({3, 8});
}

void ElevatorSystem::addCall(const Call& call) {
    calls.push_back(call);
}

void ElevatorSystem::step() {
    if (!calls.empty()) {
        Call call = calls.back();
        calls.pop_back();

        elevators[0].addTarget(call.to);
    }

    for (auto& e : elevators) {
        e.step();
    }
}