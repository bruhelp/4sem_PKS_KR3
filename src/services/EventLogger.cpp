#include "services/EventLogger.h"

void EventLogger::log(const std::string& msg) {
    std::cout << "[LOG] " << msg << std::endl;
}