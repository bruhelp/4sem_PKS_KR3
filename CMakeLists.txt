cmake_minimum_required(VERSION 3.10)

project(ElevatorSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# Все исходники проекта
file(GLOB_RECURSE PROJECT_SOURCES
    src/*.cpp
)

list(REMOVE_ITEM PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# Библиотека проекта
add_library(
    ElevatorLib
    ${PROJECT_SOURCES}
)

target_include_directories(
    ElevatorLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
    ElevatorLib PUBLIC
    Threads::Threads
)

# Основное приложение
add_executable(
    ElevatorSimulator
    src/main.cpp
)

target_link_libraries(
    ElevatorSimulator
    PRIVATE
    ElevatorLib
)

if(WIN32)
    target_link_libraries(
        ElevatorSimulator
        PRIVATE
        winmm
    )
endif()

enable_testing()

add_subdirectory(external/googletest-main)
add_subdirectory(tests)