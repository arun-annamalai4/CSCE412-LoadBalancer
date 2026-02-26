#pragma once

#include <optional>

#include "Request.h"

class WebServer {
public:
    explicit WebServer(int id);

    int id() const;
    bool isBusy() const;
    int remainingTime() const;
    int jobsCompleted() const;

    bool assignRequest(const Request& request, int cycle);
    void tick(int cycle);

    bool hasCompletedRequest() const;
    Request consumeCompletedRequest();

private:
    int id_ = 0;
    std::optional<Request> activeRequest_;
    std::optional<Request> completedRequest_;
    int remainingTime_ = 0;
    int jobsCompleted_ = 0;
};
