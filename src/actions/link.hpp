// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "settings/local.hpp"
#include "settings/global.hpp"

namespace confidant {
    namespace actions {
        namespace link {
            int linktemplate(const confidant::config::local::settings& conf, const confidant::config::global::settings& globals, const std::vector<std::string_view>& tags, bool dry);
            int linknormal(const confidant::config::local::settings& conf, const confidant::config::global::settings& globals, const std::vector<std::string_view>& tags, bool dry);
        }
    }
}