#pragma once

#include <string>
#include <vector>

/**
 * @file Config.h
 * @brief Configuration data and loading helpers for the load balancer simulation.
 */

/**
 * @brief Inclusive blocked IPv4 range represented as string endpoints.
 */
struct IpRange {
    /** @brief Starting IPv4 address in dotted format. */
    std::string startIp;
    /** @brief Ending IPv4 address in dotted format. */
    std::string endIp;
};

/**
 * @brief Runtime configuration values used to drive the simulation.
 */
struct Config {
    /** @brief Number of servers at simulation start. */
    int initialServers = 10;
    /** @brief Number of simulation cycles to execute. */
    int totalCycles = 10000;
    /** @brief Initial queue fill multiplier (servers * multiplier). */
    int initialQueueMultiplier = 100;

    /** @brief Minimum request processing time. */
    int minJobTime = 2;
    /** @brief Maximum request processing time. */
    int maxJobTime = 12;
    /** @brief Percent chance to generate arrivals in a cycle. */
    int arrivalPercent = 35;
    /** @brief Percent chance that a generated request is streaming. */
    int streamingPercent = 30;

    /** @brief Number of cycles to wait after a scale event. */
    int scaleCooldownCycles = 25;
    /** @brief Enables colored console output if supported. */
    bool colorOutput = true;
    /** @brief Deterministic seed for RNG. */
    unsigned int randomSeed = 42;

    /** @brief Output path for simulation log file. */
    std::string logFile = "logs/simulation.log";
    /** @brief Firewall blocked IP ranges. */
    std::vector<IpRange> blockedRanges;
};

/**
 * @brief Loads configuration values from a key=value text file.
 * @param filePath Path to config file.
 * @return Parsed configuration object.
 * @throws std::exception If file cannot be read or values are invalid.
 */
Config loadConfig(const std::string& filePath);

/**
 * @brief Validates constraints on configuration values.
 * @param config Config object to validate.
 * @throws std::invalid_argument If any value is out of allowed range.
 */
void validateConfig(const Config& config);
