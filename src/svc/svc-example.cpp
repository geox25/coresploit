//
// Created by geo on 1/30/24.
//

#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include "svc.hpp"

using std::cout, std::endl, std::string, std::queue;

// Just an example service for future reference
// Pass in a std::queue for relaying information
int service_example([[maybe_unused]] const vector<string>& args) {
    if (!requestRoutineStatus("example.svc")) {
        int counter = 1;

        // Example of OTHER_COLOR usage would be logging the value of a variable
        log_util.push("#O [example.svc]: init value of counter = " + std::to_string(counter));

        while (!requestRoutineStatus("example.svc")) {
            log_util.push("Information Relay " + std::to_string(counter));
            counter++;

            // Wait 0.5s before next message
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    return 0;
}