#include "EventLogger.h"

#include <iostream>
#include <filesystem>

EventLogger::EventLogger()
{
    std::filesystem::create_directories(
        "results");

    logFile.open(
        "results/events.log",
        std::ios::app);
}

EventLogger::~EventLogger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void EventLogger::log(
    const std::string& message)
{
    std::lock_guard<std::mutex> lock(
        mutex);

    std::cout
        << message
        << '\n';

    if (logFile.is_open())
    {
        logFile
            << message
            << '\n';
    }
}