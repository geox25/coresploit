//
// Created by geo on 1/27/24.
//

#ifndef PXL_TRIVIAL_BOOT_HPP
#define PXL_TRIVIAL_BOOT_HPP

#include <string>

#include "imgui.h"

namespace boot::trivial {

    // Set Style of ImGuiStyle& based on string
    void use_theme(const std::string& theme, ImGuiStyle& style);
}

#endif //PXL_TRIVIAL_BOOT_HPP