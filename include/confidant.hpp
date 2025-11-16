// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "logging.hpp"
#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <iostream>
#include <utility>

using std::cout;
using std::cerr;
using std::string;
using std::optional;
using std::vector;
using std::map;
using std::pair;

namespace fs = std::filesystem;

namespace confidant {

// represents the global config file
    struct settings {
        bool create_dirs = true; // overridden by the local setting
        int loglevel = ansi::verbosity::normal; // increases base-level verbosity, before cli options
    };

    namespace config {
    
        enum linkType { directory, file };

        struct repository {
            std::string url;
        };

        struct link {
            std::string name;
            fs::path source;
            fs::path destination;
            confidant::config::linkType type;
        };

        struct templatelink {
            std::string name;
            fs::path source;
            fs::path destination;
            std::vector<std::string> items;
        };

        /* struct linkFrom {
            fs::path from;
            fs::path to;
            std::vector<std::string> items;
        }; */

    }; // END confidant::config

    struct configuration {
        bool create_dirs;
        config::repository repo;
        vector<config::link> links;
        vector<config::templatelink> templates;
    };

    namespace debug {
        
        namespace global {
            void dumpConfig(const confidant::settings& conf);  
        };
        
        void dumpConfig(const confidant::configuration& conf);
    
    }; // END confidant::debug

    int linktemplate(const confidant::configuration& conf, const bool& dry);
    // int linkfrom(const confidant::configuration& conf, const bool& dry);
    int link(const confidant::configuration& conf, const bool& dry);

}; // END confidant