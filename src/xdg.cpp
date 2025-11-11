// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>
#include <optional>
#include <utility>
#include <map>

#include <sys/types.h>
#include <unistd.h>

#include "util.hpp"
#include "xdg.hpp"

namespace fs = std::filesystem;

namespace xdg {

std::map<std::string, std::vector<fs::path>> dirs() {
    std::map<std::string, std::vector<fs::path>> x;
    auto env_xdg_config_dirs = util::getenv("XDG_CONFIG_DIRS");
    auto env_xdg_data_dirs = util::getenv("XDG_DATA_DIRS");
    
    std::vector<fs::path> config_dirs;
    std::vector<fs::path> data_dirs;
    
    if (!env_xdg_config_dirs) {
        if (fs::exists(fs::path("/etc/local"))) {
            config_dirs.push_back("/etc/local/xdg");
        } else {
            config_dirs.push_back("/usr/local/etc/xdg");
        }
        config_dirs.push_back("/etc/xdg");
    } else {
        config_dirs = util::splitpath(env_xdg_config_dirs.value());
    }

    if (!env_xdg_data_dirs) {
        data_dirs.push_back("/usr/local/share");
        data_dirs.push_back("/usr/share");
    } else {
        data_dirs = util::splitpath(env_xdg_data_dirs.value());
    }

    x.insert(std::pair{"XDG_CONFIG_DIRS", config_dirs});
    x.insert(std::pair{"XDG_DATA_DIRS", data_dirs});

    return x;
}

std::map<std::string, fs::path> homes() {

    std::map<std::string, fs::path> x;
    auto env_home = util::getenv("HOME");
    auto env_xdg_config = util::getenv("XDG_CONFIG_HOME");
    auto env_xdg_cache = util::getenv("XDG_CACHE_HOME");
    auto env_xdg_state = util::getenv("XDG_STATE_HOME");
    auto env_xdg_data = util::getenv("XDG_DATA_HOME");

    auto env_xdg_runtime = util::getenv("XDG_RUNTIME_DIR");
    
    
    if (!env_home)
        throw std::runtime_error("HOME is not set in the environment!");
    
    std::string home = env_home.value();

    std::pair<std::string, fs::path> xdg_config;
    std::pair<std::string, fs::path> xdg_cache;
    std::pair<std::string, fs::path> xdg_state;
    std::pair<std::string, fs::path> xdg_data;
    std::pair<std::string, fs::path> xdg_runtime;
    
    if (!env_xdg_runtime) {
        int user_id = int(getuid());
        xdg_runtime = std::pair{
            "XDG_RUNTIME_DIR",
            fs::path(std::format("/run/user/{}", user_id))
        };
    } else {
        xdg_runtime = std::pair{
          "XDG_RUNTIME_DIR",
          fs::path(env_xdg_runtime.value())
        };
    }

    if (!env_xdg_config) {
        xdg_config = std::pair{
            "XDG_CONFIG_HOME",
            fs::path(std::format("{}/.config", home))
        };
    } else {
        xdg_config = std::pair{
            "XDG_CONFIG_HOME",
            fs::path(env_xdg_config.value())
        };
    }
    
    if (!env_xdg_cache) {
        xdg_cache = std::pair{
            "XDG_CACHE_HOME",
            fs::path(std::format("{}/.cache", home))
        };
    } else {
        xdg_cache = std::pair{
            "XDG_CACHE_HOME",
            fs::path(env_xdg_cache.value())
        };
    }
    
    if (!env_xdg_state) {
        xdg_state = std::pair{
            "XDG_STATE_HOME",
            fs::path(std::format("{}/.local/state", home))
        };
    } else {
        xdg_state = std::pair{
            "XDG_STATE_HOME",
            fs::path(env_xdg_state.value())
        };
    }
    
    if (!env_xdg_data) {
        xdg_data = std::pair{
            "XDG_DATA_HOME",
            fs::path(std::format("{}/.local/share", home))
        };
    } else {
        xdg_data = std::pair{
            "XDG_DATA_HOME",
            fs::path(env_xdg_data.value())
        };
    }

    x.insert(xdg_config);
    x.insert(xdg_cache);
    x.insert(xdg_state);
    x.insert(xdg_data);
    x.insert(xdg_runtime);

    return x;
}

} // NOTE: end namespace xdg