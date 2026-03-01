#include "Logger.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>

Logger::Logger(const std::string& filePath) {
    std::filesystem::path p(filePath);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }

    file_.open(filePath);
    if (!file_.is_open()) {
        throw std::runtime_error("Could not open log file: " + filePath);
    }
}

Logger::~Logger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void Logger::logCycleHeader(int cycle, std::size_t queueSize, std::size_t serverCount) {
    std::ostringstream out;
    out << "\n=== Cycle " << cycle << " ==="
        << " queue=" << queueSize
        << " servers=" << serverCount;
    writeLine(out.str());
}

void Logger::logGenerated(const Request& request) {
    ++stats_.totalGenerated;

    std::ostringstream out;
    out << "[GENERATED] requestId=" << request.id
        << " ipIn=" << request.ipIn
        << " ipOut=" << request.ipOut
        << " type=" << request.jobType
        << " time=" << request.timeRequired;
    writeLine(out.str());
}

void Logger::logArrival(const Request& request) {
    ++stats_.totalAccepted;

    std::ostringstream out;
    out << "[ARRIVAL] " << request.toString();
    writeLine(out.str());
}

void Logger::logBlocked(const Request& request, const std::string& reason) {
    ++stats_.totalBlocked;

    std::ostringstream out;
    out << "[BLOCKED] " << request.toString() << " reason=" << reason;
    writeLine(out.str());
}

void Logger::logAssigned(int serverId, const Request& request) {
    ++stats_.totalAssigned;

    std::ostringstream out;
    out << "[ASSIGN] server=" << serverId << " requestId=" << request.id;
    writeLine(out.str());
}

void Logger::logCompleted(int serverId, const Request& request) {
    ++stats_.totalCompleted;

    std::ostringstream out;
    out << "[COMPLETE] server=" << serverId << " requestId=" << request.id;
    writeLine(out.str());
}

void Logger::logScaleUp(std::size_t newServerCount, std::size_t queueSize) {
    ++stats_.scaleUps;

    std::ostringstream out;
    out << "[SCALE UP] servers=" << newServerCount << " queue=" << queueSize;
    writeLine(out.str());
}

void Logger::logScaleDown(std::size_t newServerCount, std::size_t queueSize) {
    ++stats_.scaleDowns;

    std::ostringstream out;
    out << "[SCALE DOWN] servers=" << newServerCount << " queue=" << queueSize;
    writeLine(out.str());
}

void Logger::logCycleMetrics(int cycle, std::size_t queueSize, std::size_t serverCount) {
    stats_.queueSum += static_cast<long long>(queueSize);
    if (queueSize > stats_.peakQueue) {
        stats_.peakQueue = queueSize;
    }

    std::ostringstream out;
    out << "[METRICS] cycle=" << cycle
        << " queue=" << queueSize
        << " servers=" << serverCount
        << " completed=" << stats_.totalCompleted;
    writeLine(out.str());
}

void Logger::finalizeSummary(int totalCycles) {
    double avgQueue = 0.0;
    if (totalCycles > 0) {
        avgQueue = static_cast<double>(stats_.queueSum) / static_cast<double>(totalCycles);
    }

    writeLine("\n===== FINAL SUMMARY =====");
    writeLine("totalGenerated: " + std::to_string(stats_.totalGenerated));
    writeLine("totalAccepted: " + std::to_string(stats_.totalAccepted));
    writeLine("totalBlocked: " + std::to_string(stats_.totalBlocked));
    writeLine("totalAssigned: " + std::to_string(stats_.totalAssigned));
    writeLine("totalCompleted: " + std::to_string(stats_.totalCompleted));
    writeLine("scaleUps: " + std::to_string(stats_.scaleUps));
    writeLine("scaleDowns: " + std::to_string(stats_.scaleDowns));
    writeLine("peakQueue: " + std::to_string(stats_.peakQueue));
    writeLine("averageQueue: " + std::to_string(avgQueue));
}

const SimulationStats& Logger::stats() const {
    return stats_;
}

void Logger::writeLine(const std::string& line) {
    file_ << line << '\n';
}
