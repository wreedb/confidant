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
std::string stripargz(const std::string& arg);
std::optional<std::string> getenv(const std::string& name);
bool hasenv(const std::string& name);
std::vector<fs::path> splitpath(const std::string& pathstr);
};