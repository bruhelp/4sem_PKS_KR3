#pragma once

class Elevator {
public:
    Elevator(int id);

    void step();
    void addTarget(int floor);

    int getFloor() const;

private:
    int id;
    int currentFloor;
    int targetFloor;
};