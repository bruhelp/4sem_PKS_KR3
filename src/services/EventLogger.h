#pragma once

#include <string>
#include <fstream>
#include <mutex>

class EventLogger
{
private:

    std::ofstream logFile;

    mutable std::mutex mutex;

public:

    EventLogger();

    ~EventLogger();

    void log(const std::string& message);
};