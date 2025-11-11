// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <iostream>
#include <utility>

using std::cout;
using std::cerr;
using std::string;
using std::optional;
using std::vector;
using std::map;
using std::pair;

namespace fs = std::filesystem;

namespace confidant {
namespace config {
enum linkType { directory, file };
struct repository {
    std::string url;
};
struct link {
    std::string name;
    fs::path source;
    fs::path destination;
    confidant::config::linkType type;
};
struct linkFrom {
    fs::path from;
    fs::path to;
    std::vector<std::string> items;
};
} // NOTE: end namespace confidant::config
struct configuration {
    config::repository repo;
    vector<config::link> links;
    config::linkFrom linkFrom;
};
namespace debug {
void dumpConfig(const confidant::configuration& conf);
} // NOTE: end namespace confidant::debug
} // NOTE: end namespace confidant