// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "settings.hpp"

namespace confidant {
    namespace config {
        namespace global {
            confidant::settings defaults() {
                return confidant::settings{
                    .createdirs = true,
                    .color = true,
                    .loglevel = verbose::normal
                };
            }
        };
    };
    
};