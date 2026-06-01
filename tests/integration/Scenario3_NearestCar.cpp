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
    cout << "=== Scenario 3: Nearest Car strategy ===\n";
    CallDispatcher dispatcher;
    dispatcher.setStrategy(make_unique<NearestCarStrategy>());
    ElevatorSystem system(&dispatcher, nullptr);
    
    // Лифт 1 на 1-м этаже, лифт 2 на 10-м
    auto e1 = make_unique<Elevator>(1, 5, 100, nullptr);
    auto e2 = make_unique<Elevator>(2, 5, 100, nullptr);
    // Перемещаем e2 на 10-й этаж
    e2->addDestination(10);
    while (e2->getCurrentFloor() < 10) e2->moveOneFloor();
    system.addElevator(move(e1));
    system.addElevator(move(e2));
    system.startElevators();
    
    // Вызов с 3-го этажа
    Call call(1, 3, 7, 0);
    Elevator* chosen = dispatcher.dispatchCall(system, call);
    assert(chosen->getId() == 1); // ближайший – лифт 1 (расстояние 2 против 7)
    cout << "SUCCESS: Nearest elevator selected.\n";
    return 0;
}