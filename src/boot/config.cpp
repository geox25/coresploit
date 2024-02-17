//
// Created by geo on 2/11/24.
//

#include "config.hpp"
#include "toml.hpp"

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