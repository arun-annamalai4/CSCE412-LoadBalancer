#include "LoadBalancer.h"

#include <algorithm>
#include <iostream>

/**
 * @file LoadBalancer.cpp
 * @brief Implements simulation lifecycle and autoscaling logic.
 */

LoadBalancer::LoadBalancer(const Config& config)
    : config_(config),
      firewall_(config.blockedRanges),
      generator_(config),
      logger_(config.logFile) {}

void LoadBalancer::initialize() {
    servers_.clear();
    for (int i = 0; i < config_.initialServers; ++i) {
        addServer();
    }

    const int initialQueueSize = config_.initialServers * config_.initialQueueMultiplier;
    for (int i = 0; i < initialQueueSize; ++i) {
        Request request = generator_.createRequest(0);
        logger_.logGenerated(request);
        if (!firewall_.isBlocked(request.ipIn)) {
            requestQueue_.enqueue(request);
            logger_.logArrival(request);
        } else {
            logger_.logBlocked(request, "blocked by firewall");
        }
    }

    logger_.logRunSetup(requestQueue_.size(), config_.minJobTime, config_.maxJobTime);
}

void LoadBalancer::run() {
    initialize();

    for (currentCycle_ = 0; currentCycle_ < config_.totalCycles; ++currentCycle_) {
        tick();
    }

    std::size_t activeServers = 0;
    for (const auto& server : servers_) {
        if (server->isBusy()) {
            ++activeServers;
        }
    }
    const std::size_t inactiveServers = servers_.size() - activeServers;
    const std::size_t endingQueue = requestQueue_.size();

    logger_.logEndStatus(endingQueue, activeServers, inactiveServers, endingQueue);
    logger_.finalizeSummary(config_.totalCycles);
}

void LoadBalancer::tick() {
    logger_.logCycleHeader(currentCycle_, requestQueue_.size(), servers_.size());

    processIncomingRequests();
    assignRequestsToIdleServers();
    advanceServers();
    applyScalingPolicy();

    logger_.logCycleMetrics(currentCycle_, requestQueue_.size(), servers_.size());

    if (cooldownRemaining_ > 0) {
        --cooldownRemaining_;
    }
}

void LoadBalancer::processIncomingRequests() {
    const int arrivals = generator_.randomArrivalCount();
    for (int i = 0; i < arrivals; ++i) {
        Request request = generator_.createRequest(currentCycle_);
        logger_.logGenerated(request);

        if (firewall_.isBlocked(request.ipIn)) {
            logger_.logBlocked(request, "blocked by firewall");
            continue;
        }

        requestQueue_.enqueue(request);
        logger_.logArrival(request);
    }
}

void LoadBalancer::assignRequestsToIdleServers() {
    for (std::size_t i = 0; i < servers_.size(); ++i) {
        if (requestQueue_.empty()) {
            break;
        }

        if (!servers_[i]->isBusy()) {
            Request next = requestQueue_.dequeue();
            const bool assigned = servers_[i]->assignRequest(next, currentCycle_);
            if (assigned) {
                logger_.logAssigned(servers_[i]->id(), next);
            }
        }
    }
}

void LoadBalancer::advanceServers() {
    for (std::size_t i = 0; i < servers_.size(); ++i) {
        servers_[i]->tick(currentCycle_);
        if (servers_[i]->hasCompletedRequest()) {
            Request done = servers_[i]->consumeCompletedRequest();
            logger_.logCompleted(servers_[i]->id(), done);
        }
    }
}

void LoadBalancer::applyScalingPolicy() {
    if (cooldownRemaining_ > 0) {
        return;
    }

    const std::size_t queueSize = requestQueue_.size();

    if (queueSize > highThreshold()) {
        addServer();
        logger_.logScaleUp(servers_.size(), queueSize);
        cooldownRemaining_ = config_.scaleCooldownCycles;
        return;
    }

    if (queueSize < lowThreshold()) {
        const bool removed = removeOneIdleServer();
        if (removed) {
            logger_.logScaleDown(servers_.size(), queueSize);
            cooldownRemaining_ = config_.scaleCooldownCycles;
        }
    }
}

void LoadBalancer::addServer() {
    const int newId = static_cast<int>(servers_.size()) + 1;
    servers_.push_back(std::make_unique<WebServer>(newId));
}

bool LoadBalancer::removeOneIdleServer() {
    if (servers_.size() <= 1) {
        return false;
    }

    for (int i = static_cast<int>(servers_.size()) - 1; i >= 0; --i) {
        if (!servers_[i]->isBusy()) {
            servers_.erase(servers_.begin() + i);
            return true;
        }
    }

    return false;
}

std::size_t LoadBalancer::lowThreshold() const {
    return 50U * servers_.size();
}

std::size_t LoadBalancer::highThreshold() const {
    return 80U * servers_.size();
}
