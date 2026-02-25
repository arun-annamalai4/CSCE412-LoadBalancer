#include "Config.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {
std::string trim(const std::string& input) {
    std::size_t start = 0;
    while (start < input.size() && std::isspace(static_cast<unsigned char>(input[start])) != 0) {
        ++start;
    }

    std::size_t end = input.size();
    while (end > start && std::isspace(static_cast<unsigned char>(input[end - 1])) != 0) {
        --end;
    }

    return input.substr(start, end - start);
}

bool parseBool(const std::string& value) {
    if (value == "1" || value == "true" || value == "TRUE" || value == "yes" || value == "on") {
        return true;
    }
    if (value == "0" || value == "false" || value == "FALSE" || value == "no" || value == "off") {
        return false;
    }
    throw std::invalid_argument("Invalid boolean value: " + value);
}

IpRange parseRange(const std::string& value, int lineNumber) {
    const std::size_t dashPos = value.find('-');
    if (dashPos == std::string::npos) {
        throw std::invalid_argument("Line " + std::to_string(lineNumber) + ": blocked_range must be startIp-endIp");
    }

    IpRange range{trim(value.substr(0, dashPos)), trim(value.substr(dashPos + 1))};
    if (range.startIp.empty() || range.endIp.empty()) {
        throw std::invalid_argument("Line " + std::to_string(lineNumber) + ": blocked_range contains empty IP");
    }

    return range;
}
} // namespace

Config loadConfig(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open config file: " + filePath);
    }

    Config config;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        ++lineNumber;
        const std::string stripped = trim(line);

        if (stripped.empty() || stripped[0] == '#') {
            continue;
        }

        const std::size_t eqPos = stripped.find('=');
        if (eqPos == std::string::npos) {
            throw std::invalid_argument("Line " + std::to_string(lineNumber) + ": expected key=value");
        }

        const std::string key = trim(stripped.substr(0, eqPos));
        const std::string value = trim(stripped.substr(eqPos + 1));
        if (key.empty()) {
            throw std::invalid_argument("Line " + std::to_string(lineNumber) + ": empty key");
        }

        if (key == "initial_servers") {
            config.initialServers = std::stoi(value);
        } else if (key == "total_cycles") {
            config.totalCycles = std::stoi(value);
        } else if (key == "initial_queue_multiplier") {
            config.initialQueueMultiplier = std::stoi(value);
        } else if (key == "min_job_time") {
            config.minJobTime = std::stoi(value);
        } else if (key == "max_job_time") {
            config.maxJobTime = std::stoi(value);
        } else if (key == "arrival_percent") {
            config.arrivalPercent = std::stoi(value);
        } else if (key == "streaming_percent") {
            config.streamingPercent = std::stoi(value);
        } else if (key == "scale_cooldown_cycles") {
            config.scaleCooldownCycles = std::stoi(value);
        } else if (key == "color_output") {
            config.colorOutput = parseBool(value);
        } else if (key == "random_seed") {
            config.randomSeed = static_cast<unsigned int>(std::stoul(value));
        } else if (key == "log_file") {
            config.logFile = value;
        } else if (key == "blocked_range") {
            config.blockedRanges.push_back(parseRange(value, lineNumber));
        } else {
            throw std::invalid_argument("Line " + std::to_string(lineNumber) + ": unknown key '" + key + "'");
        }
    }

    validateConfig(config);
    return config;
}

void validateConfig(const Config& config) {
    if (config.initialServers <= 0) {
        throw std::invalid_argument("initial_servers must be > 0");
    }

    if (config.totalCycles <= 0) {
        throw std::invalid_argument("total_cycles must be > 0");
    }

    if (config.initialQueueMultiplier <= 0) {
        throw std::invalid_argument("initial_queue_multiplier must be > 0");
    }

    if (config.minJobTime <= 0 || config.maxJobTime <= 0) {
        throw std::invalid_argument("job times must be > 0");
    }

    if (config.minJobTime > config.maxJobTime) {
        throw std::invalid_argument("min_job_time cannot be greater than max_job_time");
    }

    if (config.arrivalPercent < 0 || config.arrivalPercent > 100) {
        throw std::invalid_argument("arrival_percent must be between 0 and 100");
    }

    if (config.streamingPercent < 0 || config.streamingPercent > 100) {
        throw std::invalid_argument("streaming_percent must be between 0 and 100");
    }

    if (config.scaleCooldownCycles < 0) {
        throw std::invalid_argument("scale_cooldown_cycles must be >= 0");
    }

    if (config.logFile.empty()) {
        throw std::invalid_argument("log_file must not be empty");
    }
}
