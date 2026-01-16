// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <map>


namespace xdg {
    std::map<std::string, std::vector<std::filesystem::path>> dirs();
    std::map<std::string, std::filesystem::path> homes();
} // END xdg
