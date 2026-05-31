#include "EventLogger.h"

#include <iostream>
#include <chrono>
#include <iomanip>

EventLogger::EventLogger()
{
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
    std::lock_guard<std::mutex> lock(mutex);

    auto now =
        std::chrono::system_clock::now();

    auto time =
        std::chrono::system_clock::to_time_t(now);

    std::string record =
        "[" +
        std::string(std::ctime(&time));

    record.pop_back();

    record += "] ";
    record += message;

    if (logFile.is_open())
    {
        logFile
            << record
            << std::endl;
    }
}