// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string_view>
#include "settings/local.hpp"
#include "settings/global.hpp"

using sview = std::string_view;

namespace confidant {
    namespace actions {
        namespace dump {
            namespace json {
                void global(sview path);
                void local(sview path);
            }; // END json
            void global(const confidant::config::global::settings& conf);
            void local(const confidant::config::local::settings& conf);
        }; // END dump
    }; // END actions
}; // END confidant
