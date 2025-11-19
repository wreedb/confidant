// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <optional>
#include <string>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <ranges>
#include <string_view>

namespace fs = std::filesystem;
using std::vector;
using std::string;
using std::string_view;


namespace util {


    std::vector<std::string_view> split(std::string_view sv, char delimiter = '.') {
        std::vector<std::string_view> result;
        
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
    
    
    std::string stripargz(const std::string& arg) {
        std::string argz = arg;
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
        return std::string(value);
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
        auto optEnvNoColor = util::getenv("NO_COLOR");
        if (!optEnvNoColor) return true;
        else if (optEnvNoColor->empty()) return true;
        else return false;
    }


};