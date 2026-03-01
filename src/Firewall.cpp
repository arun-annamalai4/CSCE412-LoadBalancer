#include "Firewall.h"

#include <sstream>
#include <stdexcept>

Firewall::Firewall(const std::vector<IpRange>& blockedRanges) {
    for (const IpRange& range : blockedRanges) {
        addBlockedRange(range);
    }
}

void Firewall::addBlockedRange(const IpRange& blockedRange) {
    NumericIpRange numericRange;
    numericRange.start = parseIpv4ToUint(blockedRange.startIp);
    numericRange.end = parseIpv4ToUint(blockedRange.endIp);

    if (numericRange.start > numericRange.end) {
        const std::uint32_t temp = numericRange.start;
        numericRange.start = numericRange.end;
        numericRange.end = temp;
    }

    blockedRanges_.push_back(numericRange);
}

bool Firewall::isBlocked(const std::string& ip) const {
    const std::uint32_t numericIp = parseIpv4ToUint(ip);

    for (const NumericIpRange& range : blockedRanges_) {
        if (numericIp >= range.start && numericIp <= range.end) {
            return true;
        }
    }

    return false;
}

std::uint32_t Firewall::parseIpv4ToUint(const std::string& ip) {
    std::stringstream ss(ip);
    std::string part;
    int octets[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; ++i) {
        if (!std::getline(ss, part, '.')) {
            throw std::invalid_argument("Invalid IPv4 format: " + ip);
        }

        if (part.empty()) {
            throw std::invalid_argument("Invalid IPv4 format: " + ip);
        }

        int value = std::stoi(part);
        if (value < 0 || value > 255) {
            throw std::invalid_argument("IPv4 octet out of range: " + ip);
        }

        octets[i] = value;
    }

    if (std::getline(ss, part, '.')) {
        throw std::invalid_argument("Invalid IPv4 format: " + ip);
    }

    return (static_cast<std::uint32_t>(octets[0]) << 24) |
           (static_cast<std::uint32_t>(octets[1]) << 16) |
           (static_cast<std::uint32_t>(octets[2]) << 8) |
           static_cast<std::uint32_t>(octets[3]);
}
