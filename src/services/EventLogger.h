#pragma once

#include <fstream>
#include <mutex>
#include <string>

class EventLogger
{
private:
    std::ofstream logFile;

    mutable std::mutex mutex;

public:
    EventLogger();

    ~EventLogger();

    void log(
        const std::string& message);
};