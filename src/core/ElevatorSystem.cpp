#include "ElevatorSystem.h"

#include "Elevator.h"

#include "../scheduler/CallDispatcher.h"

#include "../services/EventLogger.h"

ElevatorSystem::ElevatorSystem(
    CallDispatcher *dispatcher,
    EventLogger *logger)
    : dispatcher(dispatcher),
      logger(logger)
{
}

ElevatorSystem::~ElevatorSystem()
{
    stopElevators();
}

void ElevatorSystem::addElevator(
    std::unique_ptr<Elevator> elevator)
{
    elevators.push_back(
        std::move(elevator));
}

void ElevatorSystem::startElevators()
{
    for (auto &elevator : elevators)
    {
        elevator->start();
    }
}

void ElevatorSystem::stopElevators()
{
    for (auto &elevator : elevators)
    {
        elevator->stop();
    }
}

void ElevatorSystem::addCall(
    const Call &call)
{
    {
        std::lock_guard<std::mutex> lock(
            queueMutex);

        callQueue.push(call);
    }

    if (logger)
    {
        logger->log(
            "New call: floor " + std::to_string(call.getFromFloor()) +
            " -> " +
            std::to_string(
                call.getToFloor()));
    }
}

void ElevatorSystem::processCalls()
{
    std::lock_guard<std::mutex> lock(
        queueMutex);

    while (!callQueue.empty())
    {
        Call call =
            callQueue.front();

        callQueue.pop();

        Elevator *elevator =
            dispatcher->dispatchCall(
                *this,
                call);

        if (elevator == nullptr)
        {
            continue;
        }

        elevator->addDestination(
            call.getFromFloor());

        elevator->addDestination(
            call.getToFloor());

        if (logger)
        {
            logger->log(
                "Call assigned to elevator " +
                std::to_string(
                    elevator->getId()));
        }
    }
}

std::vector<
    std::unique_ptr<Elevator>> &
ElevatorSystem::getElevators()
{
    return elevators;
}

const std::vector<
    std::unique_ptr<Elevator>> &
ElevatorSystem::getElevators() const
{
    return elevators;
}

size_t ElevatorSystem::getPendingCallCount() const
{
    std::lock_guard<std::mutex> lock(
        queueMutex);

    return callQueue.size();
}

size_t ElevatorSystem::getWaitingCount(
    int floor) const
{
    auto it =
        waitingPassengers.find(floor);

    if (it == waitingPassengers.end())
    {
        return 0;
    }

    return it->second.size();
}

void ElevatorSystem::addPassenger(
    const Passenger &passenger)
{
    waitingPassengers[passenger.getStartFloor()]
        .push_back(
            passenger);
}

void ElevatorSystem::processBoarding(
    Elevator &elevator,
    int currentSimulationTime)
{
    int floor =
        elevator.getCurrentFloor();

    auto &queue =
        waitingPassengers[floor];

    while (
        !queue.empty() &&
        elevator.hasFreeSpace())
    {
        Passenger passenger =
            queue.back();

        queue.pop_back();

        passenger.setPickupTime(
            currentSimulationTime);

        elevator.boardPassenger(
            passenger);

        if (logger)
        {
            logger->log(
                "Passenger " + std::to_string(passenger.getId()) + " entered elevator " + std::to_string(elevator.getId()));
        }
    }

    auto completed =
        elevator.unloadPassengers(
            currentSimulationTime);

    for (const auto &passenger : completed)
    {
        if (logger)
        {
            logger->log(
                "Passenger " + std::to_string(passenger.getId()) + " arrived at floor " + std::to_string(passenger.getDestinationFloor()));
        }
    }
    
    completedPassengers.insert(
        completedPassengers.end(),
        completed.begin(),
        completed.end());
}

const std::vector<Passenger> &
ElevatorSystem::getCompletedPassengers() const
{
    return completedPassengers;
}