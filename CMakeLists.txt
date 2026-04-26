cmake_minimum_required(VERSION 3.10)

project(ElevatorSimulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(ElevatorSimulator
    src/main.cpp
    src/simulation/Simulation.cpp
    src/core/Elevator.cpp
    src/core/ElevatorSystem.cpp
    src/model/Call.cpp
    src/scheduler/FIFOStrategy.cpp
    src/state/IdleState.cpp
    src/services/EventLogger.cpp
    src/result/SimulationResult.cpp
)

target_include_directories(ElevatorSimulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)