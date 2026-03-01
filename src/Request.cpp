#include "Request.h"

#include <sstream>

/**
 * @file Request.cpp
 * @brief Implements Request constructors and formatting.
 */

Request::Request(
    int requestId,
    const std::string& in,
    const std::string& out,
    int requiredTime,
    char type,
    int arrivedAtCycle)
    : id(requestId),
      ipIn(in),
      ipOut(out),
      timeRequired(requiredTime),
      jobType(type),
      arrivalCycle(arrivedAtCycle) {}

std::string Request::toString() const {
    std::ostringstream out;
    out << "Request{id=" << id
        << ", ipIn=" << ipIn
        << ", ipOut=" << ipOut
        << ", timeRequired=" << timeRequired
        << ", jobType=" << jobType
        << ", arrivalCycle=" << arrivalCycle
        << ", startCycle=" << startCycle
        << ", finishCycle=" << finishCycle
        << "}";
    return out.str();
}
