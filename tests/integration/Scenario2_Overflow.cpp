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
    cout << "=== Scenario 2: Overflow (capacity 2, 3 passengers) ===\n";
    CallDispatcher dispatcher;
    dispatcher.setStrategy(make_unique<FIFOStrategy>());
    ElevatorSystem system(&dispatcher, nullptr);
    system.addElevator(make_unique<Elevator>(1, 2, 100, nullptr)); // вместимость 2
    system.startElevators();
    
    // Три пассажира на 1 этаже
    for (int i = 1; i <= 3; ++i)
    {
        Passenger p(i, 1, 3, 0);
        system.addPassenger(p);
        system.addCall(Call(i, 1, 3, 0));
    }
    system.processCalls();
    auto& elev = system.getElevators()[0];
    
    // Лифт на 1 этаже, садим максимум 2
    system.processBoarding(*elev, 10);
    assert(elev->getPassengerCount() == 2);
    assert(system.getWaitingCount(1) == 1); // один остался
    
    // Увозим первых двух, высаживаем
    while (elev->getCurrentFloor() < 3) elev->moveOneFloor();
    system.processBoarding(*elev, 50);
    assert(elev->getPassengerCount() == 0);
    
    // Лифт возвращается за третьим
    while (elev->getCurrentFloor() > 1) elev->moveOneFloor();
    system.processBoarding(*elev, 100);
    assert(elev->getPassengerCount() == 1);
    
    while (elev->getCurrentFloor() < 3) elev->moveOneFloor();
    system.processBoarding(*elev, 150);
    assert(system.getCompletedPassengers().size() == 3);
    cout << "SUCCESS: All passengers delivered.\n";
    return 0;
}