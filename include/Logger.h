#pragma once

#include <cstddef>
#include <fstream>
#include <string>

#include "Request.h"

/**
 * @file Logger.h
 * @brief Event and summary logging for simulation output.
 */

/**
 * @brief Counters collected during one simulation run.
 */
struct SimulationStats {
    /** @brief Number of generated requests before filtering. */
    long long totalGenerated = 0;
    /** @brief Number of requests accepted into queue. */
    long long totalAccepted = 0;
    /** @brief Number of requests blocked by firewall. */
    long long totalBlocked = 0;
    /** @brief Number of requests assigned to servers. */
    long long totalAssigned = 0;
    /** @brief Number of requests completed by servers. */
    long long totalCompleted = 0;
    /** @brief Number of scale-up actions. */
    long long scaleUps = 0;
    /** @brief Number of scale-down actions. */
    long long scaleDowns = 0;
    /** @brief Running sum of queue sizes per cycle. */
    long long queueSum = 0;
    /** @brief Maximum observed queue size. */
    std::size_t peakQueue = 0;
};

/**
 * @brief Writes simulation events and summary metrics to a log file.
 */
class Logger {
public:
    /** @brief Opens or creates the output log file. */
    explicit Logger(const std::string& filePath);
    /** @brief Closes the output stream. */
    ~Logger();

    /** @brief Logs start-of-cycle state. */
    void logCycleHeader(int cycle, std::size_t queueSize, std::size_t serverCount);
    /** @brief Logs generated request event. */
    void logGenerated(const Request& request);
    /** @brief Logs accepted arrival event. */
    void logArrival(const Request& request);
    /** @brief Logs blocked request event. */
    void logBlocked(const Request& request, const std::string& reason);
    /** @brief Logs request assignment event. */
    void logAssigned(int serverId, const Request& request);
    /** @brief Logs request completion event. */
    void logCompleted(int serverId, const Request& request);
    /** @brief Logs scale up event. */
    void logScaleUp(std::size_t newServerCount, std::size_t queueSize);
    /** @brief Logs scale down event. */
    void logScaleDown(std::size_t newServerCount, std::size_t queueSize);
    /** @brief Logs cycle-level metrics. */
    void logCycleMetrics(int cycle, std::size_t queueSize, std::size_t serverCount);

    /** @brief Writes final summary statistics. */
    void finalizeSummary(int totalCycles);
    /** @brief Returns stats snapshot for read-only access. */
    const SimulationStats& stats() const;

private:
    void writeLine(const std::string& line);

    std::ofstream file_;
    SimulationStats stats_;
};
