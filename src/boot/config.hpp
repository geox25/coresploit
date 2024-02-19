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

inline const string                 CFG_DEFAULT_CS_VERSION              = "v1.3 Stable";
inline const string                 CFG_DEFAULT_CS_TITLE                = ("Coresploit " + CFG_DEFAULT_CS_VERSION);
inline const char*                  CFG_DEFAULT_CS_TITLE_CSTR()         { return CFG_DEFAULT_CS_TITLE.c_str(); }
inline constexpr ImVec2             CFG_DEFAULT_DEFAULT_WIN_CONSOLE_SIZE = {1280, 720};   // Default X,Y window size of the console
inline vector<string>               CFG_DEFAULT_SECURITY_FLAGS           = {};
inline const string                 CFG_DEFAULT_DEFAULT_THEME            = "THEME_MODERN";
inline const bool                   CFG_DEFAULT_VERBOSE                  = true;

inline string CS_VERSION;
inline string CS_TITLE;
inline const char* CS_TITLE_CSTR;
inline ImVec2 DEFAULT_WIN_CONSOLE_SIZE;
inline vector<string> SECURITY_FLAGS;
inline string DEFAULT_THEME;
inline bool VERBOSE;

// Return the front-end formatting for the default theme
string DEFAULT_THEME_FRONTEND();

// Initializes the configuration variables as seen above
void init_config();

// Saves changes to options during runtime to config.toml
void save_config();

#endif //PXL_CONFIG_HPP
