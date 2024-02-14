//
// Created by geox2 on 2/9/2024.
//

#include <string>
#include "../../svc.hpp"
#include "../../../boot/config.hpp"

static const vector<string> blacklisted_services;

bool check_routine_blacklisted(const string& routine) {
    if (std::find(SECURITY_FLAGS.begin(), SECURITY_FLAGS.end(), "#BLACKLIST " + routine) != SECURITY_FLAGS.end()) {
        return true;
    } else {
        return false;
    }
}

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
void log_security(const string& str) {
    log_system.push("#Security (security.svc) " + str);
}
void log_verbose(const string& str) {
    log_system.push("#O (security.svc) " + str);
}

// Small utility functions for code cleanliness
string get_operating_system_name()
{
#ifdef _WIN64
    return "Windows 64-bit";
#elif _WIN32
    return "Windows 32-bit";
#elif __APPLE__ || __MACH__
    return "Mac OSX";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Other";
#endif
}
string merge_security_flags() {
    if (SECURITY_FLAGS.empty()) {
        return "NONE";
    }

    string result;
    size_t count = 1;
    for (const string& flag : SECURITY_FLAGS) {
        if (count == SECURITY_FLAGS.size()) {
            result += flag;
        } else {
            result += (flag + ", ");
        }
        count++;
    }
    return result;
}

static const string INFO_PXL_VERSION      = "PXL VERSION: " + PXL_VERSION;
static const string INFO_OPERATING_SYSTEM = "HOST SYSTEM: " + get_operating_system_name();
static const string INFO_SECURITY_FLAGS   = "SECURITY_FLAGS: " + merge_security_flags();

void gather_system_info() {
    // Do something more in depth...
    log("Gathering more detailed information...");
}

void security_main() {
    // Main sequence of security.svc, will log OS and other system info
    // next will perform security checks
    log(INFO_PXL_VERSION);
    log(INFO_OPERATING_SYSTEM);
    log(INFO_SECURITY_FLAGS);

    gather_system_info();

    /*while(1) {
        // while true, do nothing for now ...
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }*/
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

