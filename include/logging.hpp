// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <format>
#include <string>
#include <string_view>
#include <iostream>
#include "util.hpp"
#include "settings.hpp"
#include "settings/global.hpp"

using std::string;
using std::string_view;

namespace ansi {

    enum color : int {
        black = 0,
        red = 1,
        green = 2,
        yellow = 3,
        blue = 4,
        magenta = 5,
        cyan = 6,
        white = 7
    };

    enum verbosity : int {
        quiet = 0,
        normal = 1,
        info = 2,
        debug = 3,
        trace = 4
    };

    std::string verbosity_literal(const int& v);

    inline bool useColor = util::hasenv("NO_COLOR") ? false : true;
    inline verbose loglevel = confidant::config::global::loglevel;

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
    
    }; // END ansi::reset

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
        std::string rgb(const int& red, const int& green, const int& blue);
    
    }; // END ansi::fg

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
        std::string rgb(const int& red, const int& green, const int& blue);
    
    }; // END ansi::bg

}; // END ansi

namespace confidant {
    
    namespace logging {
        
        std::string bolden(std::string_view str);
        std::string ital(std::string_view str);
        std::string ul(std::string_view str);
        std::string sthru(std::string_view str);
        std::string color(const int& num);
        
        namespace fg {
            
            std::string black(std::string_view s);
            std::string red(std::string_view s);
            std::string green(std::string_view s);
            std::string yellow(std::string_view s);
            std::string blue(std::string_view s);
            std::string magenta(std::string_view s);
            std::string cyan(std::string_view s);
            std::string white(std::string_view s);
        
        }; // END confidant::logging::fg
        
        namespace bg {
            
            std::string black(std::string_view s);
            std::string red(std::string_view s);
            std::string green(std::string_view s);
            std::string yellow(std::string_view s);
            std::string blue(std::string_view s);
            std::string magenta(std::string_view s);
            std::string cyan(std::string_view s);
            std::string white(std::string_view s);
        
        }; // END confidant::logging::bg
        
        template <typename... Args>
        void pretty(std::string_view msg, Args&&... fmt) {
            if (confidant::config::global::loglevel == verbose::quiet) return;
            std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
            std::cout
            << fg::magenta(">>>")
            << " "
            << fmtMsg
            << "\n";
        }
        
        template <typename... Args>
        void error(std::string_view msg, Args&&... fmt) {
            std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
            std::cerr
            << fg::red("error")
            << ": "
            << fmtMsg
            << "\n";
        }

        template <typename... Args>
        void info (std::string_view msg, Args&&... fmt) {
            if (confidant::config::global::loglevel == verbose::quiet) return;
            std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
            std::cout
            << fg::blue("info")
            << ": "
            << fmtMsg
            << "\n";
        }

        template <typename... Args>
        void warn (std::string_view msg, Args&&... fmt) {
            if (confidant::config::global::loglevel == verbose::quiet) return;
            if (confidant::config::global::loglevel < verbose::normal) return;
            std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
            std::cout
            << fg::yellow("warn")
            << ": "
            << fmtMsg
            << std::endl;
        }

        template <typename... Args>
        void warnextra (std::string_view msg, Args&&... fmt) {
            if (confidant::config::global::loglevel == verbose::quiet) return;
            if (confidant::config::global::loglevel < verbose::info) return;
            std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
            std::cout
            << fg::yellow("warn")
            << ": "
            << fmtMsg
            << std::endl;
        }

        template <typename... Args>
        void extra (std::string_view msg, Args&&... fmt) {
            if (confidant::config::global::loglevel == verbose::quiet) return;
            if (confidant::config::global::loglevel < verbose::debug) return;
            std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
            std::cout
            << fg::cyan("debug")
            << ": "
            << fmtMsg
            << std::endl;
        }

        template <typename... Args>
        [[noreturn]] void fatal(int exitCode, std::string_view msg, Args&&... fmt) {
            std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
            std::cerr
            << fg::red("fatal")
            << ": "
            << fmtMsg
            << std::endl;
            std::exit(exitCode);
        }

        
    }; // END confidant::logging

}; // END confidant