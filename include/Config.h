#pragma once

#include <string>
#include <vector>

struct IpRange {
    std::string startIp;
    std::string endIp;
};

struct Config {
    int initialServers = 10;
    int totalCycles = 10000;
    int initialQueueMultiplier = 100;

    int minJobTime = 2;
    int maxJobTime = 12;
    int arrivalPercent = 35;
    int streamingPercent = 30;

    int scaleCooldownCycles = 25;
    bool colorOutput = true;
    unsigned int randomSeed = 42;

    std::string logFile = "logs/simulation.log";
    std::vector<IpRange> blockedRanges;
};

Config loadConfig(const std::string& filePath);
void validateConfig(const Config& config);
