#pragma once

#include <memory>
#include <vector>

#include "Config.h"
#include "Firewall.h"
#include "Logger.h"
#include "RequestGenerator.h"
#include "RequestQueue.h"
#include "WebServer.h"

/**
 * @file LoadBalancer.h
 * @brief Core simulation orchestrator that manages queue, servers, and scaling.
 */

/**
 * @brief Runs the request-processing simulation and server autoscaling policy.
 */
class LoadBalancer {
public:
    /** @brief Constructs load balancer from config and dependent components. */
    explicit LoadBalancer(const Config& config);

    /** @brief Sets up initial servers and initial queue state. */
    void initialize();
    /** @brief Runs the simulation for configured number of cycles. */
    void run();
    /** @brief Executes one simulation cycle. */
    void tick();

private:
    void processIncomingRequests();
    void assignRequestsToIdleServers();
    void advanceServers();
    void applyScalingPolicy();

    void addServer();
    bool removeOneIdleServer();

    std::size_t lowThreshold() const;
    std::size_t highThreshold() const;

    Config config_;
    RequestQueue requestQueue_;
    std::vector<std::unique_ptr<WebServer>> servers_;
    Firewall firewall_;
    RequestGenerator generator_;
    Logger logger_;

    int currentCycle_ = 0;
    int cooldownRemaining_ = 0;
};
