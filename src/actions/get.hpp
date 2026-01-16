// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <variant>
#include <string>
#include <string_view>
#include <vector>
#include <optional>

#include "settings/local.hpp"

using sview = std::string_view;
using std::string;
using std::vector;
using std::optional;
using std::variant;

namespace confidant {
    namespace actions {
        namespace get {
            
            using localvalue = variant<
                bool,
                string,
                vector<string>,
                vector<confidant::config::local::link>,
                vector<confidant::config::local::templatelink>,
                confidant::config::local::link,
                confidant::config::local::templatelink
            >;
            
            using globalvalue = variant<
                bool,
                util::verbose,
                string,
                int
            >;
            
            optional<localvalue> local(const confidant::config::local::settings& conf, sview qry);
            optional<globalvalue> global(const confidant::config::global::settings& conf, sview qry);
            string formatlocalvalue(const localvalue& v);
            string formatglobalvalue(const globalvalue& v);

        }; // END get
    }; // END actions
}; // END confidant
