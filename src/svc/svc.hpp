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
#include <vector>
#include <future>

using std::atomic;
using std::string;
using std::unordered_map;
using std::function;
using std::vector;
using std::future;

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

class UnifiedService {
private:
    atomic<bool> status = true;
    function<int(const vector<string>&)> service;
public:
    explicit UnifiedService(const function<int(const vector<string>&)>& service) {
        this->service = service;
    }

    function<int(const vector<string>&)> getService() {
        return service;
    }

    atomic<bool>& getStatus() {
        return status;
    }

    // TODO: Add error-checking functionality
    bool run() {
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

// TODO: Improve log stack system
extern ThreadSafeQueue<string> log_system;      // Used for logging by system services
extern ThreadSafeQueue<string> log_util;        // Used for utility logging by mainly non-system services

extern bool addNormalRoutine(const string& id, const function<int(const vector<string>&)>& service);

extern bool requestRoutineStatus(const string& id);

extern function<int(const vector<string>&)> requestRoutine(const string& id);

extern bool requestValidRoutineID(const string& id);

extern bool requestRunRoutine(const vector<string>& id);

extern bool requestStopRoutine(const string& id);

extern void makeRoutines();

extern void start_monitor_futures();

extern void show_active_services();

#endif //PXL_SVC_HPP
