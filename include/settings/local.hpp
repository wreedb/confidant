// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "settings/global.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace confidant {
    
    namespace config {
        
        namespace local {

            enum linktype { directory, file };
            
            struct repository {
                std::string url;
            };
            
            struct link {
                std::string name;
                std::string tag;
                fs::path source;
                fs::path destination;
                linktype type;
            };
            
            struct templatelink {
                std::string name;
                std::string tag;
                fs::path source;
                fs::path destination;
                std::vector<std::string> items;
            };
            
            struct settings {
                repository repo;
                std::vector<link> links;
                std::vector<templatelink> templates;
            };
            
            confidant::config::local::settings serialize(std::string_view path, const confidant::config::global::settings& globals);
        };
    };
    
};