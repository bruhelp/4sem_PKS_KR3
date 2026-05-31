#pragma once

#include <string>

enum class AlgorithmType
{
    FIFO = 1,
    NEAREST_CAR,
    ROUND_ROBIN,
    SCAN
};

class Config
{
public:

    int floors;
    int elevators;
    int capacity;

    int passengersPerFloor;

    int travelTimePerFloor;

    int simulationDuration;

    unsigned int seed;

    AlgorithmType algorithm;

public:

    bool isValid() const;

    static Config readFromConsole();
};