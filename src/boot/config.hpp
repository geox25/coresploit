//
// Created by geox2 on 2/1/2024.
//

// PXL Boot Configuration Header File
// Modify with caution

#ifndef PXL_CONFIG_HPP
#define PXL_CONFIG_HPP

#include <string>
#include <vector>
#include "imgui.h"

using std::string, std::vector;

inline const string                 PXL_VERSION              = "v1.2 Stable";
inline const string                 PXL_TITLE                = ("PXL " + PXL_VERSION);
inline const char*                  PXL_TITLE_CSTR()         { return PXL_TITLE.c_str(); }
inline constexpr ImVec2             DEFAULT_WIN_CONSOLE_SIZE = {1280, 720};   // Default X,Y window size of the console
inline vector<string>               SECURITY_FLAGS           = {};

inline const string                 DEFAULT_THEME            = "THEME_MODERN";

// Return the front-end formatting for the default theme
string DEFAULT_THEME_FRONTEND();

// Initializes the configuration variables as seen above
void init_config();

#endif //PXL_CONFIG_HPP
