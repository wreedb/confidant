// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// #include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <iostream>
#include <utility>

#include "settings.hpp"

using std::cout;
using std::cerr;
using std::string;
using std::optional;
using std::vector;
using std::map;
using std::pair;

// namespace fs = std::filesystem;

namespace confidant {

    // represents the global config file
    // struct settings {
    //     bool create_dirs = true; // overridden by the local setting
    //     int loglevel = ansi::verbosity::normal; // increases base-level verbosity, before cli options
    //     bool color = true;
    // };
    // 
    namespace debug {
        
        namespace global {
            void dumpConfig(const confidant::settings& conf);  
        };
        
        void dumpConfig(const confidant::configuration& conf);
    
    }; // END confidant::debug

    int linktemplate(const confidant::configuration& conf, const bool& dry);
    int link(const confidant::configuration& conf, const bool& dry);

}; // END confidant