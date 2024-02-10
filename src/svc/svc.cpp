//
// Created by geo on 1/30/24.
//

#include <atomic>
#include <queue>
#include <unordered_map>
#include <vector>
#include <future>
#include "svc.hpp"

// Include system services
#include "system/security/svc-security.hpp"

// Include services
#include "svc-example.hpp"
#include "prg-ustat.hpp"

using std::atomic;
using std::string;
using std::queue;
using std::unordered_map;
using std::function;
using std::vector;
using std::future;

ThreadSafeQueue<string>                 log_system = ThreadSafeQueue<string>();
ThreadSafeQueue<string>                 log_util = ThreadSafeQueue<string>();

unordered_map<string, UnifiedService>   system_services;
unordered_map<string, UnifiedService>   services;

bool addNormalRoutine(const string& id, const function<int(const vector<string>&)>& service) {
    // Do not add if already contains id
    if (services.contains(id)) {
        return false;
    }

    // Otherwise add
    services.insert(std::make_pair(id, service));
    return true;
}

bool addSystemRoutine(const string& id, const function<int(const vector<string>&)>& service) {
    // Do not add if already contains id
    if (system_services.contains(id)) {
        return false;
    }

    // Otherwise add
    system_services.insert(std::make_pair(id, service));
    return true;
}

bool requestRoutineStatus(const string& id) {
    if (services.contains(id)) {
        return services.at(id).getStatus();
    } else {
        return true;
    }
}

function<int(const vector<string>&)> requestRoutine(const string& id) {
    return services.at(id).getService();
}

bool requestValidRoutineID(const string& id) {
    return services.contains(id);
}

bool requestRunRoutine(const string& id) {
    if (services.contains(id)) {
        return services.at(id).run();
    } else {
        return false;
    }
}

bool requestStopRoutine(const string& id) {
    if (services.contains(id)) {
        services.at(id).stop();
        return true;
    } else {
        return false;
    }
}

void makeRoutines() {
    addSystemRoutine("security.svc", service_security);

    addNormalRoutine("example.svc", service_example);
    addNormalRoutine("ustat", service_ustat);
}