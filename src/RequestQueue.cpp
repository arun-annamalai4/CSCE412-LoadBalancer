#include "RequestQueue.h"

#include <stdexcept>

void RequestQueue::enqueue(const Request& request) {
    queue_.push(request);
}

Request RequestQueue::dequeue() {
    if (queue_.empty()) {
        throw std::out_of_range("Cannot dequeue from an empty RequestQueue");
    }

    Request next = queue_.front();
    queue_.pop();
    return next;
}

const Request& RequestQueue::front() const {
    if (queue_.empty()) {
        throw std::out_of_range("Cannot read front of an empty RequestQueue");
    }

    return queue_.front();
}

bool RequestQueue::empty() const {
    return queue_.empty();
}

std::size_t RequestQueue::size() const {
    return queue_.size();
}
