//
// Created by geo on 1/30/24.
//

#include <atomic>
#include <queue>
#include <unordered_map>
#include <vector>
#include <future>
#include "svc.hpp"

// Include services
#include "svc-example.hpp"
#include "svc-ustat.hpp"

using std::atomic;
using std::string;
using std::queue;
using std::unordered_map;
using std::function;
using std::vector;
using std::future;

ThreadSafeQueue<string>                 util_log_stack = ThreadSafeQueue<string>();
unordered_map<string, UnifiedService>   services;

bool requestAddService(const string& id, const function<int(const vector<string>&)>& service) {
    // Reject request if map of service_id->UnifiedService already contains id
    if (services.contains(id)) {
        return false;
    }

    services.insert(std::make_pair(id, service));

    return true;
}

bool requestServiceStatus(const string& id) {
    if (services.contains(id)) {
        return services.at(id).getStatus();
    } else {
        return true;
    }
}

function<int(const vector<string>&)> requestService(const string& id) {
    return services.at(id).getService();
}

bool requestValidServiceID(const string& id) {
    return services.contains(id);
}

bool requestRunService(const string& id) {
    if (services.contains(id)) {
        return services.at(id).run();
    } else {
        return false;
    }
}

bool requestStopService(const string& id) {
    if (services.contains(id)) {
        services.at(id).stop();
        return true;
    } else {
        return false;
    }
}

void makeServices() {
    requestAddService("example.svc", service_example);
    requestAddService("ustat.svc", service_ustat);
}