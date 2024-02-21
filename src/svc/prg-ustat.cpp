//
// Created by geo on 2/2/24.
//

#include <iostream>
#include <string>
#include <cstring>
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

[[maybe_unused]] static std::string exec(const char* cmd) {
    char buffer[128];
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != nullptr) {
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
    log_util.push("#O [prg-ustat] <prg-ustat.cpp>: Running unix ustat code");

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
        log_util.push("#S [ustat.svc]: CPU: " + cpu_usage + "% RAM: " + ram_usage);
    } else {
        log_util.push("#E [ustat.svc]: Error trying to retrieve CPU and RAM usage info!");
        return -1;
    }

    return 0;
}
#else
int service_ustat_unix() { return -1; }
#endif

#if defined(_WIN64)
typedef long long int64_t;
typedef unsigned long long uint64_t;

/// time convert
static uint64_t file_time_2_utc(const FILETIME *ftime)
{
    LARGE_INTEGER li;

    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

// get CPU num
static int get_processor_number()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (int)info.dwNumberOfProcessors;
}

int win64_getCpuUsage(int pid)
{
    static int processor_count_ = -1;
    static int64_t last_time_ = 0;
    static int64_t last_system_time_ = 0;

    FILETIME now;
    FILETIME creation_time;
    FILETIME exit_time;
    FILETIME kernel_time;
    FILETIME user_time;
    int64_t system_time;
    int64_t time;
    int64_t system_time_delta;
    int64_t time_delta;

    int cpu = -1;

    if (processor_count_ == -1)
    {
        processor_count_ = get_processor_number();
    }

    GetSystemTimeAsFileTime(&now);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
    {
        // can not find the process
        exit(EXIT_FAILURE);
    }
    system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
    time = file_time_2_utc(&now);

    if ((last_system_time_ == 0) || (last_time_ == 0))
    {
        last_system_time_ = system_time;
        last_time_ = time;
        return win64_getCpuUsage(pid);
    }

    system_time_delta = system_time - last_system_time_;
    time_delta = time - last_time_;

    if (time_delta == 0)
    {
        return win64_getCpuUsage(pid);
    }

    cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
    last_system_time_ = system_time;
    last_time_ = time;
    return cpu;
}

// PRG-USTAT CODE TO BE RAN ON WINDOWS 64-BIT SYSTEMS
int service_ustat_win64() {
    log_util.push("#O [prg-ustat] <prg-ustat.cpp>: Running win64 ustat code");

    // Windows-specific code for memory usage
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T memoryUsage = pmc.PrivateUsage;

    // TODO: Windows-specific code for CPU usage
    int cpuUsage = win64_getCpuUsage(GetCurrentProcessId());

    log_util.push("#S [prg-ustat]: CPU Usage: " + to_string(cpuUsage) + '%' + " Memory Usage: " + to_string(memoryUsage / 1024 / 1024) + " MB");

    return 0;
}
#else
int service_ustat_win64() { return -1; }
#endif

int service_ustat([[maybe_unused]] const vector<string>& args) {

    if (!requestRoutineStatus("ustat")) {
        if (strcmp(OS, OS_UNIX) == 0) {
            log_util.push("#O [prg-ustat] <prg-ustat.cpp>: Operating System Detected As: Unix");
            service_ustat_unix();
        } else if (strcmp(OS, OS_WIN64) == 0) {
            log_util.push("#O [prg-ustat] <prg-ustat.cpp>: Operating System Detected As: Win64");
            service_ustat_win64();
        } else {
            log_util.push("#E [prg-ustat] <prg-ustat.cpp>: Unsupported Operating System Detected! Program is Operating System dependent.");
        }
    }

    return 0;
}

