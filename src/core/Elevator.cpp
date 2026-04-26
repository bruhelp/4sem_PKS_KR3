#include <iostream>
#include "core/Elevator.h"

Elevator::Elevator(int id)
    : id(id), currentFloor(0), targetFloor(0) {}

void Elevator::addTarget(int floor) {
    targetFloor = floor;
}

void Elevator::step() {
    if (currentFloor < targetFloor) currentFloor++;
    else if (currentFloor > targetFloor) currentFloor--;

    std::cout << "Elevator " << id << " at floor " << currentFloor << "\n";
}

int Elevator::getFloor() const {
    return currentFloor;
}