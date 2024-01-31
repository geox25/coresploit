//
// Created by geo on 1/30/24.
//

#include <atomic>
#include <queue>
#include <unordered_map>
#include "svc.hpp"

// Include services
#include "example-svc.hpp"

using std::atomic;
using std::string;
using std::queue;
using std::unordered_map;
using std::function;

std::atomic<bool>                       stop_service(false);
ThreadSafeQueue<string>                 util_log_stack = ThreadSafeQueue<string>();
unordered_map<string, atomic<bool>>     service_id_status = unordered_map<string, atomic<bool>>();
unordered_map<string, function<int()>>    service_id_entry = unordered_map<string, function<int()>>();

void makeServices() {
    service_id_entry["example"] = exampleService;
    service_id_status["example"] = true;
}