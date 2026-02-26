#pragma once

#include <cstdint>
#include <vector>

#include "Config.h"

struct NumericIpRange {
    std::uint32_t start = 0;
    std::uint32_t end = 0;
};

class Firewall {
public:
    Firewall() = default;
    explicit Firewall(const std::vector<IpRange>& blockedRanges);

    void addBlockedRange(const IpRange& blockedRange);
    bool isBlocked(const std::string& ip) const;

    static std::uint32_t parseIpv4ToUint(const std::string& ip);

private:
    std::vector<NumericIpRange> blockedRanges_;
};
