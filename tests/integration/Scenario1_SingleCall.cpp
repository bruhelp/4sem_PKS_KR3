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
    cout << "=== Scenario 1: Single call ===\n";
    CallDispatcher dispatcher;
    dispatcher.setStrategy(make_unique<FIFOStrategy>());
    ElevatorSystem system(&dispatcher, nullptr);
    
    // Один лифт
    system.addElevator(make_unique<Elevator>(1, 5, 100, nullptr));
    system.startElevators();
    
    // Пассажир с 1 на 5 этаж
    Passenger p(1, 1, 5, 0);
    system.addPassenger(p);
    system.addCall(Call(1, 1, 5, 0));
    system.processCalls();
    
    auto& elev = system.getElevators()[0];
    
    // Симулируем движение
    while (elev->getCurrentFloor() < 1) elev->moveOneFloor(); // уже на 1
    system.processBoarding(*elev, 0);   // посадка
    
    while (elev->getCurrentFloor() < 5) elev->moveOneFloor();
    system.processBoarding(*elev, 200); // высадка
    
    const auto& completed = system.getCompletedPassengers();
    assert(completed.size() == 1);
    assert(completed[0].getArrivalTime() == 200);
    cout << "SUCCESS: Passenger delivered.\n";
    return 0;
}