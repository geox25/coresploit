//
// Created by geo on 2/2/24.
//

#include <iostream>
#include <string>
#include "svc.hpp"

// Operating System Variables
#define OS_WIN64 "win64"
#define OS_UNIX "unix"

// Check if OS is Windows 64-bit
#if defined(_WIN64)
#define OS "win64"
#include <windows.h>
#include <psapi.h>
#include <pdh.h>
#include <pdhmsg.h>
#endif

// Check if OS is Unix (Linux, BSD, etc) but NOT macOS
#if defined(unix)
#define OS "unix"
#endif

// If OS is still not detected, raise error
#ifndef OS
#error "UNSUPPORTED OPERATING SYSTEM DETECTED"
#endif

using std::string, std::to_string;

static std::string exec(const char* cmd) {
    char buffer[128];
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

#if defined(unix)
// PRG-USTAT CODE TO BE RAN ON UNIX SYSTEMS
int service_ustat_unix() {
    util_log_stack.push("#O [prg-ustat] <prg-ustat.cpp>: Running unix ustat code");

    string pid = to_string(getpid());

    string cpu_cmd = "ps -p " + pid + " -o %cpu";
    string ram_cmd = "ps -p " + pid + " -o rss";

    string cpu_usage = exec(cpu_cmd.c_str());
    string ram_usage_str = exec(ram_cmd.c_str());

    cpu_usage.replace(cpu_usage.find("%CPU\n"), 6, "");
    cpu_usage.replace(cpu_usage.find('\n'), 2, "");

    ram_usage_str.replace(ram_usage_str.find("RSS"), 3, "");
    int ram_usage_int = std::stoi(ram_usage_str) / 1024;
    string ram_usage = to_string(ram_usage_int) + " MB";

    if (!cpu_usage.empty() && !ram_usage.empty()) {
        util_log_stack.push("#S [ustat.svc]: CPU: " + cpu_usage + "% RAM: " + ram_usage);
    } else {
        util_log_stack.push("#E [ustat.svc]: Error trying to retrieve CPU and RAM usage info!");
        return -1;
    }

    return 0;
}
#else
int service_ustat_unix() { return -1; }
#endif

#if defined(_WIN64)


// PRG-USTAT CODE TO BE RAN ON WINDOWS 64-BIT SYSTEMS
int service_ustat_win64() {
    util_log_stack.push("#O [prg-ustat] <prg-ustat.cpp>: Running win64 ustat code");

    // Windows-specific code for memory usage
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T memoryUsage = pmc.PrivateUsage;

    // TODO: Windows-specific code for CPU usage

    util_log_stack.push("#S [prg-ustat]: Memory Usage: " + to_string(memoryUsage / 1024 / 1024) + " MB");

    return 0;
}
#else
int service_ustat_win64() { return -1; }
#endif

int service_ustat(const vector<string>& args) {

    if (!requestRoutineStatus("ustat")) {
        if (OS == OS_UNIX) {
            util_log_stack.push("#O [prg-ustat] <prg-ustat.cpp>: Operating System Detected As: Unix");
            service_ustat_unix();
        } else if (OS == OS_WIN64) {
            util_log_stack.push("#O [prg-ustat] <prg-ustat.cpp>: Operating System Detected As: Win64");
            service_ustat_win64();
        } else {
            util_log_stack.push("#E [prg-ustat] <prg-ustat.cpp>: Unsupported Operating System Detected! Program is Operating System dependent.");
        }
    }

    return 0;
}

