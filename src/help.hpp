// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <string_view>

using std::string;
using sview = std::string_view;

namespace help {
    namespace general {
        void help(sview argz);
        void usage(sview argz);
    }; // END general

    namespace init {
        void help(sview argz);
    }; // END init

    namespace config {
        void help(sview argz);
        namespace dump {
            void help(sview argz);
        }; // END dump
        namespace get {
            void help(sview argz);
        }; // END get
    }; // END config

    namespace link {
        void help(sview argz);
    }; // END link

    namespace defaults {
        string global_config_path();
        string global_config();
        void write_global_config(sview p);
        string local_config(sview argz);
        void write_local_config(sview dir);
    }; // END defaults
}; // END help
