//
// Created by geox2 on 2/9/2024.
//

#include <string>
#include "../../svc.hpp"

// Small log functions to avoid unnecessary repetition of concatenation
void log(const string& str) {
    log_system.push("#N (security.svc) " + str);
}
void log_error(const string& str) {
    log_system.push("#E (security.svc) " + str);
}
void log_warning(const string& str) {
    log_system.push("#W (security.svc) " + str);
}
void log_verbose(const string& str) {
    log_system.push("#O (security.svc) " + str);
}

void security_main() {
    // Main sequence of security.svc, will log OS and other system info
    // next will perform security checks
}

int service_security(const vector<string>& args) {
    // No conditional checking whether it is supposed to run or not because
    // SECURITY.SVC SHOULD ALWAYS BE RUNNING!

    try {
        // Call main security.svc function
        log("Calling main security.svc function...");
        security_main();
    } catch (const std::exception& ex) {
        log_error(ex.what());
    } catch (...) {
        log_error("UNKNOWN ERROR!");
    }

    return 0;
}

