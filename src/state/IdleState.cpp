#include "state/IdleState.h"
#include "core/Elevator.h"
#include <iostream>

void IdleState::handle(Elevator& e) {
    std::cout << "Elevator idle\n";
}