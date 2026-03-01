#include "RequestGenerator.h"

#include <stdexcept>

RequestGenerator::RequestGenerator(const Config& config)
    : minJobTime_(config.minJobTime),
      maxJobTime_(config.maxJobTime),
      arrivalPercent_(config.arrivalPercent),
      streamingPercent_(config.streamingPercent),
      rng_(config.randomSeed) {}

bool RequestGenerator::shouldGenerateThisCycle() {
    std::uniform_int_distribution<int> percent(1, 100);
    return percent(rng_) <= arrivalPercent_;
}

int RequestGenerator::randomArrivalCount() {
    if (!shouldGenerateThisCycle()) {
        return 0;
    }

    
    std::uniform_int_distribution<int> countDist(1, 3);
    return countDist(rng_);
}

Request RequestGenerator::createRequest(int cycle) {
    Request request(
        nextRequestId_,
        randomIp(),
        randomIp(),
        randomJobTime(),
        randomJobType(),
        cycle);

    ++nextRequestId_;
    return request;
}

std::string RequestGenerator::randomIp() {
    std::uniform_int_distribution<int> octetDist(1, 254);
    return std::to_string(octetDist(rng_)) + "." +
           std::to_string(octetDist(rng_)) + "." +
           std::to_string(octetDist(rng_)) + "." +
           std::to_string(octetDist(rng_));
}

char RequestGenerator::randomJobType() {
    std::uniform_int_distribution<int> percent(1, 100);
    if (percent(rng_) <= streamingPercent_) {
        return 'S';
    }
    return 'P';
}

int RequestGenerator::randomJobTime() {
    if (minJobTime_ > maxJobTime_) {
        throw std::invalid_argument("minJobTime cannot be greater than maxJobTime");
    }

    std::uniform_int_distribution<int> timeDist(minJobTime_, maxJobTime_);
    return timeDist(rng_);
}
