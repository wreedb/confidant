// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <variant>

#include "settings/local.hpp"

namespace confidant {
    namespace actions {
        namespace get {
            
            using localvalue = std::variant<
                bool,
                std::string,
                std::vector<std::string>,
                std::vector<confidant::config::local::link>,
                std::vector<confidant::config::local::templatelink>,
                confidant::config::local::link,
                confidant::config::local::templatelink
            >;
            
            using globalvalue = std::variant<
                bool,
                util::verbose,
                std::string,
                int
            >;
            
            std::optional<localvalue> local(const confidant::config::local::settings& conf, std::string_view qry);
            std::optional<globalvalue> global(const confidant::config::global::settings& conf, std::string_view qry);
            std::string formatlocalvalue(const localvalue& v);
            std::string formatglobalvalue(const globalvalue& v);
        }
    }
}