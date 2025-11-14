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
    }; // END ucl::var

    bool check(const ucl::Ucl& obj, const std::string& key);
    int members(const ucl::Ucl& object);

}; // END ucl

namespace confidant {
    std::string substitute(const std::string& tmpl, const std::string& item);
    namespace config {
        confidant::configuration serialize(std::string_view path, const confidant::settings& globals);
        namespace global {
            confidant::settings serialize(std::string_view path);
        }; // END confidant::global
    }; // END confidant::config
}; // END confidant