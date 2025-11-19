// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>

namespace util {
    std::vector<std::string_view> split(std::string_view sv, char delimiter = '.');
    bool hasperms(std::string_view p);
    std::string stripargz(const std::string& arg);
    std::optional<std::string> getenv(const std::string& name);
    bool hasenv(const std::string& name);
    std::string unexpandhome(std::string_view p);
    std::vector<std::filesystem::path> splitpath(const std::string& pathstr);
    bool usecolorp();
};