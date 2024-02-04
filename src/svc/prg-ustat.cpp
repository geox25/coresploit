//
// Created by geo on 2/2/24.
//

#include <iostream>
#include <string>
#include "svc.hpp"

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

int service_ustat(const vector<string>& args) {

    if (!requestRoutineStatus("ustat")) {
        string pid = to_string(getpid());

        string cpu_cmd = "ps -p " + pid + " -o %cpu";
        string ram_cmd = "ps -p " + pid + " -o rss";

        string cpu_usage = exec(cpu_cmd.c_str());
        string ram_usage_str = exec(ram_cmd.c_str());

        cpu_usage.replace(cpu_usage.find("%CPU\n"), 6, "");
        cpu_usage.replace(cpu_usage.find("\n"), 2, "");

        ram_usage_str.replace(ram_usage_str.find("RSS"), 3, "");
        int ram_usage_int = std::stoi(ram_usage_str) / 1024;
        string ram_usage = to_string(ram_usage_int) + " MB";

        if (cpu_usage != "" && ram_usage != "") {
            util_log_stack.push("#S [ustat.svc]: CPU: " + cpu_usage + "% RAM: " + ram_usage);
        } else {
            util_log_stack.push("#E [ustat.svc]: Error trying to retrieve CPU and RAM usage info!");
            return -1;
        }
    }

    return 0;
}

