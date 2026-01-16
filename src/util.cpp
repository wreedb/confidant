// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <optional>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <filesystem>
#include <ranges>
#include <string_view>
#include <map>

#include "parse.hpp"
#include "xdg.hpp"
#include "msg.hpp"
#include "util.hpp"

namespace fs = std::filesystem;
using std::vector;
using std::string;
using std::string_view;

namespace util {
    
    std::map<std::string, std::string> makevarmap(std::string_view path) {
        
        std::map<std::string, std::string> vars;
        std::map<std::string, fs::path> xdghomes = xdg::homes();

        ucl::var::add("XDG_CONFIG_HOME", xdghomes["XDG_CONFIG_HOME"].string(), vars);
        ucl::var::add("XDG_CACHE_HOME",  xdghomes["XDG_CACHE_HOME"].string(),  vars);
        ucl::var::add("XDG_DATA_HOME",   xdghomes["XDG_DATA_HOME"].string(),   vars);
        ucl::var::add("XDG_STATE_HOME",  xdghomes["XDG_STATE_HOME"].string(),  vars);
        
        ucl::var::add("xdg_config_home", xdghomes["XDG_CONFIG_HOME"].string(), vars);
        ucl::var::add("xdg_cache_home",  xdghomes["XDG_CACHE_HOME"].string(),  vars);
        ucl::var::add("xdg_data_home",   xdghomes["XDG_DATA_HOME"].string(),   vars);
        ucl::var::add("xdg_state_home",  xdghomes["XDG_STATE_HOME"].string(),  vars);
        
        ucl::var::add("XDG_RUNTIME_DIR", xdghomes["XDG_RUNTIME_DIR"].string(), vars);
        ucl::var::add("xdg_runtime_dir", xdghomes["XDG_RUNTIME_DIR"].string(), vars);
 
        std::optional<std::string> homedir = util::getenv("HOME");
        if (!homedir)
            throw std::runtime_error("HOME is not set in the environment!");
     
        ucl::var::add("HOME", homedir.value(), vars);
        ucl::var::add("home", homedir.value(), vars);
        ucl::var::add("REPO", fs::path(path).parent_path().string(), vars);
        ucl::var::add("repo", fs::path(path).parent_path().string(), vars);
        
        std::optional<std::string> username = util::getenv("USER");
        if (username) {
            ucl::var::add("user", username.value(), vars);
            ucl::var::add("USER", username.value(), vars);
        }
        
        std::optional<std::string> email = util::getenv("EMAIL");
        if (email) {
            ucl::var::add("email", email.value(), vars);
            ucl::var::add("EMAIL", email.value(), vars);
        }
        
        return vars;
    }
    
    std::string verboseliteral(verbose v) {
        switch (v) {
            case verbose::quiet:
                return "quiet";
            case verbose::normal:
                return "normal";
            case verbose::info:
                return "info";
            case verbose::debug:
                return "debug";
            case verbose::trace:
                return "trace";
            default:
                msg::fatal("log-level {} is invalid!", int(v));
        }
        
    }

    std::string substitute(std::string_view tmpl, std::string_view item) {
        const std::string placeholder = "%{item}";
        std::string result = tmpl.data();
        
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.length(), item);
            pos += item.length();
        }
        return result;
    }

    std::vector<std::string_view> split(std::string_view sv) {
        std::vector<std::string_view> result;
        
        char delimiter = '.';
        
        size_t start = 0;
        size_t end = sv.find(delimiter);
        
        while (end != std::string_view::npos) {
            result.push_back(sv.substr(start, end - start));
            start = end + 1;
            end = sv.find(delimiter, start);
        }
        
        // Add the last (or only) part
        result.push_back(sv.substr(start));
        
        return result;
    }
    
    std::vector<std::string_view> splittags(std::string_view sv) {
        std::vector<std::string_view> result;
        
        char delimiter = ',';
        
        size_t start = 0;
        size_t end = sv.find(delimiter);
        
        while (end != std::string_view::npos) {
            result.push_back(sv.substr(start, end - start));
            start = end + 1;
            end = sv.find(delimiter, start);
        }
        
        // Add the last (or only) part
        result.push_back(sv.substr(start));
        
        return result;
    }
    
    bool hasperms(std::string_view p)
    {
        fs::path path = fs::path(p);
        fs::path pp = path.parent_path();
        if (pp.empty()) pp = ".";
        if (!fs::exists(pp)) return false;
        fs::perms perms = fs::status(pp).permissions();
        return (perms & fs::perms::owner_write) != fs::perms::none;
    }
    
    
    std::string stripargz(std::string_view arg) {
        std::string argz = arg.data();
        // remove './' from beginning of string
        if (argz.size() >= 2 && argz.substr(0, 2) == "./")
            argz = argz.substr(2);
        // like 'basename'
        size_t last_slash = argz.find_last_of("/");
        if (last_slash != std::string::npos)
            argz = argz.substr(last_slash + 1);
        return argz;
    }

    std::optional<std::string> getenv(const std::string& name) {
        char* value = std::getenv(name.c_str());
        if (value == nullptr) return std::nullopt;
        else return std::string(value);
    }

    bool hasenv(const std::string& name) {
        char* value = std::getenv(name.c_str());
        return value == nullptr ? false : true;
    }

    std::string unexpandhome(std::string_view p) {
        fs::path path = fs::path(p);
        std::string res = std::string(p);
        auto opthome = util::getenv("HOME");
        if (!opthome) return std::string(p);
    
        std::string_view home = opthome.value();
    
        if (res.starts_with(home)) return "~" + res.substr(home.length());
        else return res;
    }

    std::vector<fs::path> splitpath(const std::string& pathstr) {
        return pathstr
            | std::views::split(':')
            | std::views::transform([](auto&& part) {
                  return fs::path(std::string_view(part.begin(), part.end()));
              })
            | std::ranges::to<std::vector>();
    }
    
    bool usecolorp() {
        std::optional<std::string> envnocolor = util::getenv("NO_COLOR");
        bool tty = (isatty(STDOUT_FILENO) == 1);
        if (!envnocolor && tty) return true;
        else if (!envnocolor && !tty) return false;
        else if (envnocolor->empty()) return true;
        else return false;
    }


}; // END util
