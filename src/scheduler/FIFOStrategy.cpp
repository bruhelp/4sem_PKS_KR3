#include "scheduler/FIFOStrategy.h"

Call FIFOStrategy::getNext(const std::vector<Call>& calls) {
    return calls.front();
}