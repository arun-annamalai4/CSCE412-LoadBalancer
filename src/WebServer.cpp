#include "WebServer.h"

#include <stdexcept>

WebServer::WebServer(int id) : id_(id) {}

int WebServer::id() const {
    return id_;
}

bool WebServer::isBusy() const {
    return activeRequest_.has_value();
}

int WebServer::remainingTime() const {
    return remainingTime_;
}

int WebServer::jobsCompleted() const {
    return jobsCompleted_;
}

bool WebServer::assignRequest(const Request& request, int cycle) {
    if (isBusy()) {
        return false;
    }

    activeRequest_ = request;
    activeRequest_->startCycle = cycle;
    remainingTime_ = request.timeRequired;
    return true;
}

void WebServer::tick(int cycle) {
    if (!isBusy()) {
        return;
    }

    if (remainingTime_ > 0) {
        --remainingTime_;
    }

    if (remainingTime_ == 0) {
        activeRequest_->finishCycle = cycle;
        completedRequest_ = *activeRequest_;
        activeRequest_.reset();
        ++jobsCompleted_;
    }
}

bool WebServer::hasCompletedRequest() const {
    return completedRequest_.has_value();
}

Request WebServer::consumeCompletedRequest() {
    if (!completedRequest_.has_value()) {
        throw std::out_of_range("No completed request available to consume");
    }

    Request finished = *completedRequest_;
    completedRequest_.reset();
    return finished;
}
