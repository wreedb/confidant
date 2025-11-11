// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <optional>
#include <string>
#include <cstdlib>
#include <vector>
#include <filesystem>
#include <ranges>

namespace fs = std::filesystem;

namespace util {

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

std::vector<fs::path> splitpath(const std::string& pathstr) {
    return pathstr
        | std::views::split(':')
        | std::views::transform([](auto&& part) {
              return fs::path(std::string_view(part.begin(), part.end()));
          })
        | std::ranges::to<std::vector>();
}



};