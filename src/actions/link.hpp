// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string_view>
#include <vector>

#include "settings/local.hpp"
#include "settings/global.hpp"

using sview = std::string_view;
using std::vector;

namespace confidant {
    namespace actions {
        namespace link {
            int linktemplate(const confidant::config::local::settings& conf, const confidant::config::global::settings& globals, const vector<sview>& tags, bool dry);
            int linknormal(const confidant::config::local::settings& conf, const confidant::config::global::settings& globals, const vector<sview>& tags, bool dry);
        }; // END link
    }; // END actions
}; // END confidant
