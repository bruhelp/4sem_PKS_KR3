cmake_minimum_required(VERSION 3.10)

project(ElevatorSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(ElevatorSimulator ${SOURCES})

target_include_directories(ElevatorSimulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ElevatorSimulator PRIVATE
    Threads::Threads
)

if(WIN32)
    target_link_libraries(ElevatorSimulator PRIVATE
        winmm
    )
endif()