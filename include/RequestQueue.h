#pragma once

#include <cstddef>
#include <queue>

#include "Request.h"

class RequestQueue {
public:
    void enqueue(const Request& request);
    Request dequeue();
    const Request& front() const;

    bool empty() const;
    std::size_t size() const;

private:
    std::queue<Request> queue_;
};
