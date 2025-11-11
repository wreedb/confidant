// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <iostream>
#include <fstream>
#include <string>
#include <print>
#include <sstream>
#include <map>
#include <ucl++.h>

#include "confidant.hpp"

using std::cerr;
using std::println;
using std::string;
using std::ifstream;
using std::stringstream;
using std::optional;
using std::map;

namespace ucl {
namespace var {
std::map<std::string, std::string> add(const std::string& name, const std::string& value, std::map<std::string, std::string>& vm);
std::map<std::string, std::string> fromenv(const std::string& key, std::map<std::string, std::string>& vm, const std::string& fallback);
}; // NOTE: end namespace ucl::var

bool check(const ucl::Ucl& obj, const std::string& key);
int members(const ucl::Ucl& object);

}; // END ucl

namespace confidant {
namespace config {
confidant::configuration serialize(const string& path);
} // NOTE: end namespace confidant::config
} // NOTE: end namespace confidant