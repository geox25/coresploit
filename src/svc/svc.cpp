//
// Created by geo on 1/30/24.
//

#include <atomic>
#include <queue>
#include <unordered_map>
#include <vector>
#include <future>
#include "svc.hpp"

// Include config
#include "../boot/config.hpp"

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

// PUBLIC VARIABLES
ThreadSafeQueue<string>                 log_system = ThreadSafeQueue<string>();
ThreadSafeQueue<string>                 log_util = ThreadSafeQueue<string>();
unordered_map<string, UnifiedService>   services;

// PRIVATE VARIABLES
unordered_map<string, future<int>>      system_services_futures;
unordered_map<string, UnifiedService>   system_services;

bool addNormalRoutine(const string& id, const function<int(const vector<string>&)>& service) {
    // Do not add if already contains id
    if (services.contains(id)) {
        return false;
    }

    // Otherwise add
    services.insert(std::make_pair(id, service));
    return true;
}

bool addSystemRoutine(const string& id, const function<int(const vector<string>&)>& service, const bool& auto_start) {
    // Do not add if already contains id
    if (system_services.contains(id)) {
        return false;
    }

    // Otherwise add
    system_services.insert(std::make_pair(id, service));

    // If routine should start automatically
    if (auto_start) {
        system_services.at(id).run();
        system_services_futures.emplace(id, std::async(std::launch::async, [id] { return system_services.at(id).getService()(SECURITY_ARGS); }));
    }

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

// This function will be run in monitor_futures() thread in boot.cpp
// It does not have its own thread in order to save resources
void monitor_system_futures() {
    for (auto it = system_services_futures.begin(); it != system_services_futures.end(); ) {
        if (it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            string service_id = it->first;
            requestStopRoutine(service_id);
            it = system_services_futures.erase(it);
            if (service_id == "security.svc") {
                log_system.push("#Security [monitor_system_futures()] <svc.cpp>: security.svc has been removed from system_services_futures! This is usually a result of an early return due to an error. Please restart PXL.");
            } else {
                log_system.push("#O [monitor_system_futures()] <svc.cpp>: [" + service_id +
                                "] has been erased from system futures");
            }
            log_system.push("#O [monitor_system_futures()] <svc.cpp>: System Futures Length: " + std::to_string(system_services_futures.size()));
        } else {
            ++it;
        }
    }
}

void makeRoutines() {
    addSystemRoutine("security.svc", service_security, true);

    addNormalRoutine("example.svc", service_example);
    addNormalRoutine("ustat", service_ustat);
}