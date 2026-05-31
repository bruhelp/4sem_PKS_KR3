#pragma once

class ElevatorSystem;

class ConsoleRenderer
{
public:

    void render(
        const ElevatorSystem& elevatorSystem,
        int floors,
        int currentTime,
        int simulationDuration) const;

private:

    void clearScreen() const;
};