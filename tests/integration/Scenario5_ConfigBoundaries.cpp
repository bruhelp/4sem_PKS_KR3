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

void runWithConfig(const Config& cfg)
{
    cout << "Testing config: floors=" << cfg.floors
         << ", elevators=" << cfg.elevators
         << ", capacity=" << cfg.capacity << "\n";
    CallDispatcher dispatcher;
    dispatcher.setStrategy(make_unique<FIFOStrategy>());
    ElevatorSystem system(&dispatcher, nullptr);
    for (int i = 1; i <= cfg.elevators; ++i)
        system.addElevator(make_unique<Elevator>(i, cfg.capacity, cfg.travelTimePerFloor, nullptr));
    system.startElevators();
    
    // Один пассажира для проверки работоспособности
    Passenger p(1, 1, cfg.floors, 0);
    system.addPassenger(p);
    system.addCall(Call(1, 1, cfg.floors, 0));
    system.processCalls();
    
    auto& e = system.getElevators()[0];
    while (e->getCurrentFloor() < 1) e->moveOneFloor();
    system.processBoarding(*e, 0);
    while (e->getCurrentFloor() < cfg.floors) e->moveOneFloor();
    system.processBoarding(*e, 100);
    
    assert(system.getCompletedPassengers().size() == 1);
    cout << "  OK\n";
}

int main()
{
    cout << "=== Scenario 5: Config boundary values ===\n";
    Config minCfg;
    minCfg.floors = 2;
    minCfg.elevators = 1;
    minCfg.capacity = 1;
    minCfg.passengersPerFloor = 1;
    minCfg.travelTimePerFloor = 100;
    minCfg.simulationDuration = 1;
    minCfg.seed = 1;
    minCfg.algorithm = AlgorithmType::FIFO;
    runWithConfig(minCfg);
    
    Config maxCfg;
    maxCfg.floors = 50;
    maxCfg.elevators = 6;
    maxCfg.capacity = 20;
    maxCfg.passengersPerFloor = 50;
    maxCfg.travelTimePerFloor = 100;
    maxCfg.simulationDuration = 1;
    maxCfg.seed = 2;
    maxCfg.algorithm = AlgorithmType::SCAN;
    runWithConfig(maxCfg);
    
    cout << "SUCCESS: Boundary configs work.\n";
    return 0;
}