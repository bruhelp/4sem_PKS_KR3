#pragma once

class Call
{
private:

    int passengerId;

    int fromFloor;

    int toFloor;

    int timestamp;

public:

    Call(
        int passengerId,
        int fromFloor,
        int toFloor,
        int timestamp);

    int getPassengerId() const;

    int getFromFloor() const;

    int getToFloor() const;

    int getTimestamp() const;
};