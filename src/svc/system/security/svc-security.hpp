//
// Created by geox2 on 2/9/2024.
//

#ifndef PXL_SVC_SECURITY_HPP
#define PXL_SVC_SECURITY_HPP

#include <string>
#include <vector>

using std::string, std::vector;

bool check_routine_blacklisted(const string& routine);

int service_security(const vector<string>& args);

#endif //PXL_SVC_SECURITY_HPP
