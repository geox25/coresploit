//
// Created by geo on 1/30/24.
//

#ifndef PXL_SVC_HPP
#define PXL_SVC_HPP

#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <functional>

using std::atomic;
using std::string;
using std::unordered_map;
using std::function;

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> queue;
    std::mutex mutex;
    std::condition_variable cond;

public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(value);
        cond.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        while(queue.empty()) {
            cond.wait(lock);
        }
        T value = queue.front();
        queue.pop();
        return value;
    }

    T front() {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.front();
    }

    T back() {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.back();
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.size();
    }
};

extern atomic<bool>                             stop_service;
extern ThreadSafeQueue<string>                  util_log_stack;

class UnifiedService {
private:
    atomic<bool> status = true;
    function<int()> service;
public:
    explicit UnifiedService(const function<int()>& service) {
        this->service = service;
    }

    function<int()> getService() {
        return service;
    }

    atomic<bool>& getStatus() {
        return status;
    }

    // TODO: Add error-checking functionality
    bool start() {
        status = false;
        return true;
    }

    // TODO: Add error-checking functionality
    bool stop() {
        status = true;
        return true;
    }
};

// TODO: Replace string with more robust ID class?
extern unordered_map<string, UnifiedService> services;

extern bool requestAddService(const string& id, const UnifiedService& service);

extern bool requestServiceStatus(const string& id);

extern function<int()> requestService(const string& id);

extern bool requestValidServiceID(const string& id);

extern bool requestStartService(const string& id);

extern bool requestStopService(const string& id);

void makeServices();

#endif //PXL_SVC_HPP
