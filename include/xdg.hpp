// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <map>


namespace fs = std::filesystem;

namespace xdg {
std::map<std::string, std::vector<fs::path>> dirs();
std::map<std::string, fs::path> homes();
} // NOTE: end namespace xdg