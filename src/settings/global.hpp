// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <cstdlib>
#include <string>
#include "util.hpp"

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
            struct settings {
                bool color = true;
                bool createdirs = true;
                util::verbose loglevel = util::verbose::normal;
            };
            
            // confidant::settings defaults();
            inline bool color = true;
            inline bool createdirs = true;
            inline util::verbose loglevel = util::verbose::normal;
            
            confidant::config::global::settings serialize(std::string_view path);
        }; // END confidant::config::global
    }; // END confidant::config
}; // END confidant