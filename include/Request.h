#pragma once

#include <string>

struct Request {
    int id = 0;
    std::string ipIn;
    std::string ipOut;
    int timeRequired = 0;
    char jobType = 'P';
    int arrivalCycle = 0;
    int startCycle = -1;
    int finishCycle = -1;

    Request() = default;
    Request(
        int requestId,
        const std::string& in,
        const std::string& out,
        int requiredTime,
        char type,
        int arrivedAtCycle);

    std::string toString() const;
};
