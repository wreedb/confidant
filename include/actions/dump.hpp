// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "settings/local.hpp"
#include "settings/global.hpp"

namespace confidant {
    namespace actions {
        namespace dump {
            void global(const confidant::config::global::settings& conf);
            void local(const confidant::config::local::settings& conf);
        } // END confidant::actions::dump
    } // END confidant::actions
} // END confidant