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
int exampleService() {
    int counter = 1;

    while (!requestServiceStatus("example.svc")) {
        util_log_stack.push("Information Relay " + std::to_string(counter));
        counter++;

        // Wait 0.5s before next message
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}