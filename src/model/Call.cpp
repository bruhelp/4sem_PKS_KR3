#include "Call.h"

Call::Call(
    int passengerId,
    int fromFloor,
    int toFloor,
    int timestamp)
    :
    passengerId(passengerId),
    fromFloor(fromFloor),
    toFloor(toFloor),
    timestamp(timestamp)
{
}

int Call::getPassengerId() const
{
    return passengerId;
}

int Call::getFromFloor() const
{
    return fromFloor;
}

int Call::getToFloor() const
{
    return toFloor;
}

int Call::getTimestamp() const
{
    return timestamp;
}