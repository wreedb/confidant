// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <string>
#include <string_view>

namespace help {
    namespace general {
        void help(std::string_view argz);
        void usage(std::string_view argz);
    };

    namespace init {
        void help(std::string_view argz);
    };

    namespace config {
        void help(std::string_view argz);
        namespace dump {
            void help(std::string_view argz);
        };
        namespace get {
            void help(std::string_view argz);
        };
    };

    namespace link {
        void help(std::string_view argz);
    };

    namespace defaults {
        std::string global_config_path();
        std::string global_config();
        void write_global_config(std::string_view p);
        std::string local_config(std::string_view argz);
        void write_local_config(std::string_view dir);
    };
};