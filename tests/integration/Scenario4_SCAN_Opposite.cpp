#include <iostream>
#include <cassert>
#include "../../src/core/Elevator.h"
#include "../../src/core/ElevatorSystem.h"
#include "../../src/scheduler/FIFOStrategy.h"
#include "../../src/scheduler/NearestCarStrategy.h"
#include "../../src/scheduler/SCANStrategy.h"
#include "../../src/model/Passenger.h"
#include "../../src/model/Call.h"
#include "../../src/services/EventLogger.h"
#include "../../src/services/StatisticsCollector.h"
#include "../../src/scheduler/CallDispatcher.h"

using namespace std;

int main()
{
    cout << "=== Scenario 4: SCAN strategy, opposite direction ===\n";
    CallDispatcher dispatcher;
    dispatcher.setStrategy(make_unique<SCANStrategy>());
    ElevatorSystem system(&dispatcher, nullptr);

    auto e1 = make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = make_unique<Elevator>(2, 5, 100, nullptr);

    // Поднимаем e1 на 10-й этаж (выше этажа вызова)
    e1->addDestination(10);
    while (e1->getCurrentFloor() < 10)
        e1->moveOneFloor();

    e1->addDestination(12);

    // e2 остаётся свободным на 1-м этаже
    system.addElevator(move(e1));
    system.addElevator(move(e2));
    system.startElevators();

    // Вызов с 5-го этажа вниз (на 1-й этаж)
    Call call(1, 5, 1, 0);
    Elevator *chosen = dispatcher.dispatchCall(system, call);

    assert(chosen->getId() == 2);
    cout << "SUCCESS: SCAN picked idle elevator over incompatible moving one.\n";
    return 0;
}