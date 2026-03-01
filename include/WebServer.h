#pragma once

#include <optional>

#include "Request.h"

/**
 * @file WebServer.h
 * @brief WebServer model that processes one request at a time.
 */

/**
 * @brief Represents a single simulated server worker.
 */
class WebServer {
public:
    /** @brief Creates a server with an integer identifier. */
    explicit WebServer(int id);

    /** @brief Gets the server identifier. */
    int id() const;
    /** @brief Returns true when a request is currently being processed. */
    bool isBusy() const;
    /** @brief Returns remaining cycles for the active request. */
    int remainingTime() const;
    /** @brief Total count of completed requests for this server. */
    int jobsCompleted() const;

    /**
     * @brief Assigns a new request if the server is idle.
     * @param request Request to process.
     * @param cycle Current simulation cycle for start timestamp.
     * @return True when assignment succeeds.
     */
    bool assignRequest(const Request& request, int cycle);

    /**
     * @brief Advances processing by one cycle.
     * @param cycle Current simulation cycle for completion timestamp.
     */
    void tick(int cycle);

    /** @brief Indicates whether a completed request is ready to be consumed. */
    bool hasCompletedRequest() const;

    /**
     * @brief Returns and clears the completed request buffer.
     * @throws std::out_of_range If no completed request exists.
     */
    Request consumeCompletedRequest();

private:
    int id_ = 0;
    std::optional<Request> activeRequest_;
    std::optional<Request> completedRequest_;
    int remainingTime_ = 0;
    int jobsCompleted_ = 0;
};
