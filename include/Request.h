#pragma once

#include <string>

/**
 * @file Request.h
 * @brief Defines the Request data structure used by the simulation queue and servers.
 */

/**
 * @brief Represents one web request moving through the load balancer system.
 */
struct Request {
    /** @brief Unique request identifier. */
    int id = 0;
    /** @brief Source/requester IP address. */
    std::string ipIn;
    /** @brief Destination/result IP address. */
    std::string ipOut;
    /** @brief Processing duration in cycles. */
    int timeRequired = 0;
    /** @brief Job type: 'P' for processing or 'S' for streaming. */
    char jobType = 'P';
    /** @brief Cycle when request is generated. */
    int arrivalCycle = 0;
    /** @brief Cycle when request gets assigned to a server. */
    int startCycle = -1;
    /** @brief Cycle when request finishes processing. */
    int finishCycle = -1;

    /** @brief Default constructor. */
    Request() = default;

    /**
     * @brief Constructs a request with simulation fields.
     */
    Request(
        int requestId,
        const std::string& in,
        const std::string& out,
        int requiredTime,
        char type,
        int arrivedAtCycle);

    /**
     * @brief Serializes request fields into a readable one-line format.
     * @return String representation of this request.
     */
    std::string toString() const;
};
