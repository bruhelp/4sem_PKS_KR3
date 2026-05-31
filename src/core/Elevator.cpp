#include "Elevator.h"

#include <chrono>
#include <algorithm>

#include "../state/IElevatorState.h"
#include "../state/IdleState.h"

#include "../services/EventLogger.h"

Elevator::Elevator(
    int id,
    int capacity,
    int travelTimePerFloor,
    EventLogger *logger)
    : id(id),
      currentFloor(1),
      capacity(capacity),
      travelTimePerFloor(travelTimePerFloor),
      direction(Direction::Idle),
      logger(logger),
      running(false),
      doorsOpen(false),
      movementTime(0)
{
    state = std::make_unique<IdleState>();
}

Elevator::~Elevator()
{
    stop();
}

void Elevator::start()
{
    if (running)
    {
        return;
    }

    running = true;

    worker = std::thread(
        &Elevator::processLoop,
        this);
}

void Elevator::stop()
{
    running = false;

    cv.notify_all();

    if (worker.joinable())
    {
        worker.join();
    }
}

void Elevator::processLoop()
{
    while (running)
    {
        {
            std::unique_lock<std::mutex> lock(mutex);

            cv.wait_for(
                lock,
                std::chrono::milliseconds(100));
        }

        if (!running)
        {
            break;
        }

        state->handle(*this);
    }
}

void Elevator::setState(
    std::unique_ptr<IElevatorState> newState)
{
    std::lock_guard<std::mutex> lock(mutex);

    state = std::move(newState);
}

void Elevator::addDestination(int floor)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        destinations.push(floor);
    }

    cv.notify_one();

    if (logger)
    {
        logger->log(
            "Elevator " + std::to_string(id) + " received destination " + std::to_string(floor));
    }
}

bool Elevator::hasDestinations() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return !destinations.empty();
}

int Elevator::getPendingStopsCount() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return static_cast<int>(
        destinations.size());
}

void Elevator::moveOneFloor()
{
    int targetFloor;

    {
        std::lock_guard<std::mutex> lock(mutex);

        if (destinations.empty())
        {
            direction = Direction::Idle;
            return;
        }

        targetFloor = destinations.front();
    }

    if (targetFloor > currentFloor)
    {
        direction = Direction::Up;
    }
    else if (targetFloor < currentFloor)
    {
        direction = Direction::Down;
    }
    else
    {
        return;
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds(
            travelTimePerFloor));

    {
        std::lock_guard<std::mutex> lock(mutex);

        if (direction == Direction::Up)
        {
            ++currentFloor;
        }
        else
        {
            --currentFloor;
        }

        movementTime +=
            travelTimePerFloor;
    }

    if (logger)
    {
        logger->log(
            "Elevator " + std::to_string(id) + " moved to floor " + std::to_string(currentFloor));
    }
}

bool Elevator::isOnTargetFloor() const
{
    std::lock_guard<std::mutex> lock(mutex);

    if (destinations.empty())
    {
        return false;
    }

    return currentFloor ==
           destinations.front();
}

int Elevator::getTargetFloor() const
{
    std::lock_guard<std::mutex> lock(mutex);

    if (destinations.empty())
    {
        return currentFloor;
    }

    return destinations.front();
}

void Elevator::handleStop()
{
    doorsOpen = true;

    if (logger)
    {
        logger->log(
            "Elevator " + std::to_string(id) + " opened doors on floor " + std::to_string(currentFloor));
    }

    unloadPassengers();

    std::this_thread::sleep_for(
        std::chrono::milliseconds(1500));
    serviceTime += 1500;

    {
        std::lock_guard<std::mutex> lock(mutex);

        if (!destinations.empty())
        {
            destinations.pop();
        }
    }

    doorsOpen = false;

    if (logger)
    {
        logger->log(
            "Elevator " + std::to_string(id) + " closed doors on floor " + std::to_string(currentFloor));
    }
}

void Elevator::loadPassenger(
    const Passenger &passenger)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (static_cast<int>(
            passengers.size()) >= capacity)
    {
        return;
    }

    passengers.push_back(passenger);
}

void Elevator::unloadPassengers()
{
    passengers.erase(
        std::remove_if(
            passengers.begin(),
            passengers.end(),
            [this](const Passenger &passenger)
            {
                return passenger
                           .getDestinationFloor() == currentFloor;
            }),
        passengers.end());
}

int Elevator::getCurrentFloor() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return currentFloor;
}

int Elevator::getId() const
{
    return id;
}

int Elevator::getPassengerCount() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return static_cast<int>(
        passengers.size());
}

int Elevator::getCapacity() const
{
    return capacity;
}

Direction Elevator::getDirection() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return direction;
}

bool Elevator::isDoorOpen() const
{
    return doorsOpen;
}

long long Elevator::getMovementTime() const
{
    return movementTime;
}

std::string Elevator::getStateName() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return state->getName();
}

bool Elevator::hasFreeSpace() const
{
    std::lock_guard<std::mutex> lock(mutex);

    return static_cast<int>(
               passengers.size()) < capacity;
}

void Elevator::boardPassenger(
    Passenger passenger)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (
        static_cast<int>(
            passengers.size()) >= capacity)
    {
        return;
    }

    passengers.push_back(
        passenger);

    destinations.push(
        passenger.getDestinationFloor());
}

std::vector<Passenger>
Elevator::unloadPassengers(
    int currentSimulationTime)
{
    std::lock_guard<std::mutex> lock(mutex);

    std::vector<Passenger> unloaded;

    auto iterator =
        passengers.begin();

    while (
        iterator !=
        passengers.end())
    {
        if (
            iterator
                ->getDestinationFloor() ==
            currentFloor)
        {
            iterator
                ->setArrivalTime(
                    currentSimulationTime);

            unloaded.push_back(
                *iterator);

            iterator =
                passengers.erase(
                    iterator);
        }
        else
        {
            ++iterator;
        }
    }

    return unloaded;
}

std::vector<int>
Elevator::getDestinationList() const
{
    std::vector<int> result;

    std::queue<int> copy =
        destinations;

    while (!copy.empty())
    {
        result.push_back(
            copy.front());

        copy.pop();
    }

    return result;
}

int Elevator::getCurrentTarget() const
{
    if (destinations.empty())
    {
        return -1;
    }

    return destinations.front();
}

long long Elevator::getServiceTime() const
{
    return serviceTime;
}