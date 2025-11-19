// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <vector>
#include <string_view>

namespace fs = std::filesystem;

enum verbose : int {
    quiet = 0,
    normal = 1,
    info = 2,
    debug = 3,
    trace = 4
};

namespace confidant {
    namespace config {
        struct repository{
            std::string url;
        };
        enum linkType { directory, file };
        
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
    };
    
    struct settings {
        bool createdirs;
        bool color;
        verbose loglevel;
    };
    
    struct configuration {
        bool create_dirs;
        config::repository repo;
        std::vector<config::link> links;
        std::vector<config::templatelink> templates;
    };
    
    namespace config {
        namespace local {
            configuration serialize(std::string_view path, const confidant::settings& globals);
        }
        namespace global {
            confidant::settings serialize(std::string_view path);
        };
    };
};