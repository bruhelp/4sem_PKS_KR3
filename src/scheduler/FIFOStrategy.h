#pragma once
#include <vector>
#include "model/Call.h"

class FIFOStrategy {
public:
    Call getNext(const std::vector<Call>& calls);
};