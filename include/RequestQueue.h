#pragma once

#include <cstddef>
#include <queue>

#include "Request.h"

/**
 * @file RequestQueue.h
 * @brief FIFO wrapper for Request objects.
 */

/**
 * @brief Queue abstraction used by the load balancer to store pending requests.
 */
class RequestQueue {
public:
    /**
     * @brief Adds a request to the back of the queue.
     */
    void enqueue(const Request& request);

    /**
     * @brief Removes and returns the front request.
     * @throws std::out_of_range If the queue is empty.
     */
    Request dequeue();

    /**
     * @brief Returns the front request without removing it.
     * @throws std::out_of_range If the queue is empty.
     */
    const Request& front() const;

    /** @brief Checks whether the queue has no elements. */
    bool empty() const;
    /** @brief Returns the number of queued requests. */
    std::size_t size() const;

private:
    std::queue<Request> queue_;
};
