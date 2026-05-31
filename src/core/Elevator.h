#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <string>

#include "../model/Direction.h"
#include "../model/Passenger.h"
#include "../model/Call.h"

class IElevatorState;
class EventLogger;

class Elevator
{
private:
    int id;

    int currentFloor;

    int capacity;

    int travelTimePerFloor;

    Direction direction;

    std::vector<Passenger> passengers;

    std::queue<int> destinations;

    std::unique_ptr<IElevatorState> state;

    EventLogger *logger;

    std::thread worker;

    mutable std::mutex mutex;

    std::condition_variable cv;

    std::atomic<bool> running;

    std::atomic<bool> doorsOpen;

    long long movementTime;

private:
    void processLoop();

public:
    Elevator(
        int id,
        int capacity,
        int travelTimePerFloor,
        EventLogger *logger);

    ~Elevator();

    Elevator(const Elevator &) = delete;

    Elevator &operator=(const Elevator &) = delete;

    void start();

    void stop();

    void setState(
        std::unique_ptr<IElevatorState> newState);

    void addDestination(int floor);

    bool hasDestinations() const;

    int getPendingStopsCount() const;

    void moveOneFloor();

    void handleStop();

    bool isOnTargetFloor() const;

    int getTargetFloor() const;

    void loadPassenger(
        const Passenger &passenger);

    void unloadPassengers();

    int getCurrentFloor() const;

    int getId() const;

    int getPassengerCount() const;

    int getCapacity() const;

    Direction getDirection() const;

    bool isDoorOpen() const;

    long long getMovementTime() const;

    std::string getStateName() const;

    void boardPassenger(
        Passenger passenger);

    std::vector<Passenger>
    unloadPassengers(
        int currentSimulationTime);

    bool hasFreeSpace() const;
};