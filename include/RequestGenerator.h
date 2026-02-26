#pragma once

#include <random>
#include <string>

#include "Config.h"
#include "Request.h"

class RequestGenerator {
public:
    explicit RequestGenerator(const Config& config);

    bool shouldGenerateThisCycle();
    Request createRequest(int cycle);
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
