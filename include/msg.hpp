// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <format>
#include <string>
#include <iostream>
#include <string_view>

#include "util.hpp"
#include "fmt.hpp"
#include "settings/global.hpp"

namespace gconf = confidant::config::global;
namespace fg = fmt::fg;

using util::verbose;

namespace msg {
    template <typename... Args>
    void pretty(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cout
        << fg::magenta(">>>")
        << " "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void error(std::string_view msg, Args&&... fmt) {
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cout
        << fg::red("error")
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void info(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cout
        << fg::blue("info")
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void warn(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet || gconf::loglevel <= verbose::normal) return;
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cout
        << fg::yellow("warn")
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void warnextra(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        if (gconf::loglevel <= verbose::info) return;
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cout
        << fg::yellow("warn")
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void extra(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet || gconf::loglevel < verbose::debug) return;
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cout
        << fg::cyan("debug")
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void debug(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet || gconf::loglevel < verbose::debug) return;
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cout
        << fg::cyan("debug")
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void trace(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet || gconf::loglevel < verbose::trace) return;
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cout
        << fg::cyan("trace")
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    [[noreturn]] void fatal(int ecode, std::string_view msg, Args&&... fmt) {
        std::string message = std::vformat(msg.data(), std::make_format_args(fmt...));
        std::cerr
        << fg::red("fatal")
        << ": "
        << message
        << std::endl;
        std::exit(ecode);
    }
    
    // use exit code 1 by default
    template <typename... Args>
    [[noreturn]] void fatal(std::string_view msg, Args&&... fmt) {
        fatal(1, msg, std::forward<Args>(fmt)...);
    }
    
    
}; // END msg