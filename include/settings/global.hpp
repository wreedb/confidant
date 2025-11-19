#pragma once

#include <cstdlib>
#include <string>

#include "settings.hpp"

inline bool usecolor = []() {
    char* value = std::getenv("NO_COLOR");
    if (value == nullptr || std::string(value).empty()) {
        return true;
    } else {
        return false;
    }
}();

namespace confidant {
    namespace config {
        namespace global {
            confidant::settings defaults();
            inline bool color = defaults().color;
            inline bool createdirs = defaults().createdirs;
            inline verbose loglevel = defaults().loglevel;
        }; // END confidant::config::global
    }; // END confidant::config
}; // END confidant