//
// Created by geo on 2/11/24.
//

#include <iostream>
#include "config.hpp"
#include "toml.hpp"

const string FULL_CONFIG_PATH = "config.toml";

// Return the front-end formatting for the default theme
string DEFAULT_THEME_FRONTEND() {
    if (DEFAULT_THEME == "THEME_MODERN") {
        return "Modern";
    } else if (DEFAULT_THEME == "THEME_RETRO") {
        return "Retro";
    } else if (DEFAULT_THEME == "THEME_GRAY") {
        return "Gray";
    } else {
        return "Modern";
    }
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

    // access with STL-like manner
    if(!config_data.contains("Metadata")) {
        config_data["Metadata"]["title"] = "Coresploit Configuration";
    }

    // Write TOML data to file
    std::ofstream file(FULL_CONFIG_PATH);
    file << (config_data);
    file.close();
}
