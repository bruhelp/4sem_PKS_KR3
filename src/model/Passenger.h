#pragma once

class Passenger
{
private:

    int id;

    int startFloor;

    int destinationFloor;

    int requestTime;

    int pickupTime;

    int arrivalTime;

public:

    Passenger(
        int id,
        int startFloor,
        int destinationFloor,
        int requestTime);

    int getId() const;

    int getStartFloor() const;

    int getDestinationFloor() const;

    int getRequestTime() const;

    int getPickupTime() const;

    int getArrivalTime() const;

    void setPickupTime(
        int pickupTime);

    void setArrivalTime(
        int arrivalTime);

    int getWaitingTime() const;

    int getRideTime() const;
};