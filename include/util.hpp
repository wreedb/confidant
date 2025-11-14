// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace util {
bool perms_to_link(const fs::path& p);
bool perms_to_write(const fs::path& p);
std::string stripargz(const std::string& arg);
std::optional<std::string> getenv(const std::string& name);
bool hasenv(const std::string& name);
std::string unexpandhome(std::string_view p);
std::vector<fs::path> splitpath(const std::string& pathstr);
};