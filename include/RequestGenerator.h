#pragma once

#include <random>
#include <string>

#include "Config.h"
#include "Request.h"

/**
 * @file RequestGenerator.h
 * @brief Random request generation utilities.
 */

/**
 * @brief Creates requests and arrival events from configured probabilities.
 */
class RequestGenerator {
public:
    /** @brief Builds generator from configuration values and seed. */
    explicit RequestGenerator(const Config& config);

    /** @brief Returns true if this cycle should generate arrivals. */
    bool shouldGenerateThisCycle();
    /** @brief Builds one request object for the current cycle. */
    Request createRequest(int cycle);
    /** @brief Number of new requests to generate this cycle. */
    int randomArrivalCount();

private:
    std::string randomIp();
    char randomJobType();
    int randomJobTime();

    int nextRequestId_ = 1;
    int minJobTime_ = 1;
    int maxJobTime_ = 1;
    int arrivalPercent_ = 0;
    int streamingPercent_ = 0;
    std::mt19937 rng_;
};
