#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <mutex>

#include "../model/Call.h"
#include <unordered_map>
#include "../model/Passenger.h"

class Elevator;

class EventLogger;

class CallDispatcher;

class ElevatorSystem
{
private:
    std::vector<
        std::unique_ptr<Elevator>>
        elevators;

    std::queue<Call> callQueue;

    mutable std::mutex queueMutex;

    CallDispatcher *dispatcher;

    EventLogger *logger;

    std::unordered_map<
        int,
        std::vector<Passenger>>
        waitingPassengers;

    std::vector<Passenger>
        completedPassengers;

public:
    ElevatorSystem(
        CallDispatcher *dispatcher,
        EventLogger *logger);

    ~ElevatorSystem();

    void addElevator(
        std::unique_ptr<Elevator> elevator);

    void addCall(
        const Call &call);

    void processCalls();

    void startElevators();

    void stopElevators();

    const std::vector<
        std::unique_ptr<Elevator>> &
    getElevators() const;

    std::vector<
        std::unique_ptr<Elevator>> &
    getElevators();

    size_t getPendingCallCount() const;

    size_t getWaitingCount(
        int floor) const;

    void addPassenger(
        const Passenger &passenger);

    void processBoarding(
        Elevator &elevator,
        int currentSimulationTime);

    const std::vector<Passenger> &
    getCompletedPassengers() const;
};