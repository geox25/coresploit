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
    while(1) {
        // while true, do nothing for now ...
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

int service_security(const vector<string>& args) {
    // No conditional checking whether it is supposed to run or not because
    // SECURITY.SVC SHOULD ALWAYS BE RUNNING!

    try {
        // Call main security.svc function
        log("Calling main security.svc function...");
        log("Have you ever read a webpage or document that used this text without paying much attention to it? The lorem ipsum is a placeholder text used in publishing and graphic design. This filler text is a short paragraph that contains all the letters of the alphabet. The characters are spread out evenly so that the reader's attention is focused on the layout of the text instead of its content. Many software programs and applications have made it their default dummy text. Since the lorem ipsum is always used as a placeholder text, its use indicates that this is not a final version of a document, thus helping to avoid unnecessary printing.");
        security_main();
    } catch (const std::exception& ex) {
        log_error(ex.what());
    } catch (...) {
        log_error("UNKNOWN ERROR!");
    }

    return 0;
}

