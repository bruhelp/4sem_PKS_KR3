#include "Passenger.h"

Passenger::Passenger(
    int id,
    int startFloor,
    int destinationFloor,
    int requestTime)
    :
    id(id),
    startFloor(startFloor),
    destinationFloor(destinationFloor),
    requestTime(requestTime),
    pickupTime(-1),
    arrivalTime(-1)
{
}

int Passenger::getId() const
{
    return id;
}

int Passenger::getStartFloor() const
{
    return startFloor;
}

int Passenger::getDestinationFloor() const
{
    return destinationFloor;
}

int Passenger::getRequestTime() const
{
    return requestTime;
}

int Passenger::getPickupTime() const
{
    return pickupTime;
}

int Passenger::getArrivalTime() const
{
    return arrivalTime;
}

void Passenger::setPickupTime(
    int pickupTime)
{
    this->pickupTime = pickupTime;
}

void Passenger::setArrivalTime(
    int arrivalTime)
{
    this->arrivalTime = arrivalTime;
}

int Passenger::getWaitingTime() const
{
    if (pickupTime < 0)
    {
        return 0;
    }

    return pickupTime - requestTime;
}

int Passenger::getRideTime() const
{
    if (arrivalTime < 0)
    {
        return 0;
    }

    return arrivalTime - pickupTime;
}