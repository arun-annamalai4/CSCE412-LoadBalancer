#pragma once

#include <cstdint>
#include <vector>

#include "Config.h"

/**
 * @file Firewall.h
 * @brief IP range filtering for simulated DOS/firewall behavior.
 */

/**
 * @brief Numeric representation of an inclusive IPv4 range.
 */
struct NumericIpRange {
    /** @brief Numeric start address. */
    std::uint32_t start = 0;
    /** @brief Numeric end address. */
    std::uint32_t end = 0;
};

/**
 * @brief Firewall component that blocks requests from configured ranges.
 */
class Firewall {
public:
    /** @brief Default constructor with no blocked ranges. */
    Firewall() = default;
    /** @brief Constructs firewall from configured blocked ranges. */
    explicit Firewall(const std::vector<IpRange>& blockedRanges);

    /** @brief Adds one blocked IP range. */
    void addBlockedRange(const IpRange& blockedRange);
    /** @brief Checks if the given IPv4 address should be blocked. */
    bool isBlocked(const std::string& ip) const;

    /**
     * @brief Converts dotted IPv4 string into 32-bit integer.
     * @throws std::invalid_argument If format is invalid.
     */
    static std::uint32_t parseIpv4ToUint(const std::string& ip);

private:
    std::vector<NumericIpRange> blockedRanges_;
};
