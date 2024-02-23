//
// Created by geo on 2/11/24.
//

#include <iostream>
#include "config.hpp"
#include "toml.hpp"

using std::string;

const string FULL_CONFIG_PATH = "config.toml";

// Return the front-end formatting for the default theme
string DEFAULT_THEME_FRONTEND() {
    if (DEFAULT_THEME == "THEME_UNREAL") {return "Unreal";}
    if (DEFAULT_THEME == "THEME_MICROSOFT") {return "Microsoft";}
    if (DEFAULT_THEME == "THEME_MATERIAL_FLAT") {return "Material Flat";}
    if (DEFAULT_THEME == "THEME_FUTURISTIC") {return "Futuristic";}
    if (DEFAULT_THEME == "THEME_MODERN") {return "Modern";}
    if (DEFAULT_THEME == "THEME_RETRO") {return "Retro";}
    if (DEFAULT_THEME == "THEME_GRAY") {return "Gray";}

    return "Modern";
}

int init_config_file(const string& file_dir) {
    std::fstream config_file;

    // Open the file in out|app mode. If the file doesn't exist, it will be created.
    config_file.open(file_dir, std::fstream::out | std::fstream::app);

    // Flush the file
    config_file.flush();

    // Close the file
    config_file.close();

    return 0;
}

// Initializes the configuration variables as seen in config.hpp
void init_config() {
    // If config.toml doesn't exist then create it
    init_config_file(FULL_CONFIG_PATH);

    // TOML data from file in root directory of project
    toml::value config_data;
    try {
        config_data = toml::parse(FULL_CONFIG_PATH);
    } catch (const std::runtime_error& error) {
        std::cout << error.what() << std::endl;
    }

    // If Metadata section is not present in config, we need
    // to initialize all default configuration values
    if(!config_data.contains("Metadata")) {
        // Initialize default metadata values
        config_data["Metadata"]["title"] = "Coresploit Configuration";
        config_data["Metadata"]["cs_version"] = CFG_DEFAULT_CS_VERSION;
        config_data["Metadata"]["cs_title"] = CFG_DEFAULT_CS_TITLE;

        // Initialize default actual configuration values
        config_data["Config"]["default_win_console_size"] = {CFG_DEFAULT_DEFAULT_WIN_CONSOLE_SIZE[0], CFG_DEFAULT_DEFAULT_WIN_CONSOLE_SIZE[1]};
        config_data["Config"]["security_flags"] = CFG_DEFAULT_SECURITY_FLAGS;
        config_data["Config"]["default_theme"] = CFG_DEFAULT_DEFAULT_THEME;
        config_data["Config"]["verbose"] = CFG_DEFAULT_VERBOSE;
    }

    // This will set the values of the config variables used in the program to the values that are used in the config
    // If they are not set by the user in the config, the default values will be used from the conditional above.
    // Otherwise, if they are changed by the user, it will use the custom values in config.toml
    // Get the tables
    const auto metadata = toml::find(config_data, "Metadata");
    const auto config = toml::find(config_data, "Config");
    const auto default_win_console_size_arr = toml::find(config, "default_win_console_size");

    CS_VERSION = toml::find<string>(metadata, "cs_version");
    CS_TITLE = toml::find<string>(metadata, "cs_title");
    CS_TITLE_CSTR = CS_TITLE.c_str();
    DEFAULT_WIN_CONSOLE_SIZE = ImVec2(toml::find<float>(default_win_console_size_arr, 0), toml::find<float>(default_win_console_size_arr, 1));
    SECURITY_FLAGS = toml::find<vector<string>>(config, "security_flags");
    DEFAULT_THEME = toml::find<string>(config, "default_theme");
    VERBOSE = toml::find<bool>(config, "verbose");

    // Write TOML data to file
    std::ofstream file(FULL_CONFIG_PATH);
    file << (config_data);
    file.close();
}

void save_config() {
    std::cout << "saving config" << std::endl;

    // TOML data from file in root directory of project
    toml::value config_data;
    try {
        config_data = toml::parse(FULL_CONFIG_PATH);
    } catch (const std::runtime_error& error) {
        std::cout << error.what() << std::endl;
    }

    config_data["Config"]["verbose"] = VERBOSE;
    config_data["Config"]["default_theme"] = DEFAULT_THEME;

    // Write TOML data to file
    std::ofstream file(FULL_CONFIG_PATH);
    file << (config_data);
    file.close();
}
