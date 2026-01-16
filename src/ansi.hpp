// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string_view>

namespace ansi {
    constexpr std::string_view freset     = "\033[0m";
    constexpr std::string_view bold       = "\033[1m";
    constexpr std::string_view dim        = "\033[2m";
    constexpr std::string_view italic     = "\033[3m";
    constexpr std::string_view underline  = "\033[4m";
    constexpr std::string_view blink      = "\033[5m";
    constexpr std::string_view hidden     = "\033[8m";
    constexpr std::string_view strikethru = "\033[9m";
    
    namespace reset {
        constexpr std::string_view bold       = "\033[22m";
        constexpr std::string_view dim        = "\033[22m";
        constexpr std::string_view italic     = "\033[23m";
        constexpr std::string_view underline  = "\033[24m";
        constexpr std::string_view blink      = "\033[25m";
        constexpr std::string_view hidden     = "\033[28m";
        constexpr std::string_view strikethru = "\033[29m";
    };
    
    namespace fg {
        constexpr std::string_view black   = "\033[30m";
        constexpr std::string_view red     = "\033[31m";
        constexpr std::string_view green   = "\033[32m";
        constexpr std::string_view yellow  = "\033[33m";
        constexpr std::string_view blue    = "\033[34m";
        constexpr std::string_view magenta = "\033[35m";
        constexpr std::string_view cyan    = "\033[36m";
        constexpr std::string_view white   = "\033[37m";
        constexpr std::string_view reset   = "\033[39m";
    };
    
    namespace bg {
        constexpr std::string_view black   = "\033[40m";
        constexpr std::string_view red     = "\033[41m";
        constexpr std::string_view green   = "\033[42m";
        constexpr std::string_view yellow  = "\033[43m";
        constexpr std::string_view blue    = "\033[44m";
        constexpr std::string_view magenta = "\033[45m";
        constexpr std::string_view cyan    = "\033[46m";
        constexpr std::string_view white   = "\033[47m";
        constexpr std::string_view reset   = "\033[49m";
    };
};