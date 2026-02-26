#pragma once

#include <memory>
#include <vector>

#include "Config.h"
#include "Firewall.h"
#include "Logger.h"
#include "RequestGenerator.h"
#include "RequestQueue.h"
#include "WebServer.h"

class LoadBalancer {
public:
    explicit LoadBalancer(const Config& config);

    void initialize();
    void run();
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
