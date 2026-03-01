#pragma once

#include <cstddef>
#include <fstream>
#include <string>

#include "Request.h"

struct SimulationStats {
    long long totalGenerated = 0;
    long long totalAccepted = 0;
    long long totalBlocked = 0;
    long long totalAssigned = 0;
    long long totalCompleted = 0;
    long long scaleUps = 0;
    long long scaleDowns = 0;
    long long queueSum = 0;
    std::size_t peakQueue = 0;
};

class Logger {
public:
    explicit Logger(const std::string& filePath);
    ~Logger();

    void logCycleHeader(int cycle, std::size_t queueSize, std::size_t serverCount);
    void logGenerated(const Request& request);
    void logArrival(const Request& request);
    void logBlocked(const Request& request, const std::string& reason);
    void logAssigned(int serverId, const Request& request);
    void logCompleted(int serverId, const Request& request);
    void logScaleUp(std::size_t newServerCount, std::size_t queueSize);
    void logScaleDown(std::size_t newServerCount, std::size_t queueSize);
    void logCycleMetrics(int cycle, std::size_t queueSize, std::size_t serverCount);

    void finalizeSummary(int totalCycles);
    const SimulationStats& stats() const;

private:
    void writeLine(const std::string& line);

    std::ofstream file_;
    SimulationStats stats_;
};
