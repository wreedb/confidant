// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <format>
#include <string>
#include <iostream>
#include <string_view>

#include "i18n.hpp"
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
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cout
        << fg::magenta(">>>")
        << " "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void error(std::string_view msg, Args&&... fmt) {
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cout
        << fg::red(_("error"))
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void info(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cout
        << fg::blue(_("info"))
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void warn(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        if (gconf::loglevel <= verbose::normal) return;
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cout
        << fg::yellow(_("warn"))
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void warnextra(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        if (gconf::loglevel <= verbose::info) return;
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cout
        << fg::yellow(_("warn"))
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void extra(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        if (gconf::loglevel < verbose::debug) return;
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cout
        << fg::cyan(_("debug"))
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void debug(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        if (gconf::loglevel < verbose::debug) return;
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cout
        << fg::cyan(_("debug"))
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    void trace(std::string_view msg, Args&&... fmt) {
        if (gconf::loglevel == verbose::quiet) return;
        if (gconf::loglevel < verbose::trace) return;
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cout
        << fg::cyan(_("trace"))
        << ": "
        << message
        << "\n";
    }
    
    template <typename... Args>
    [[noreturn]] void fatal(std::string_view msg, Args&&... fmt) {
        std::string translated = _(msg.data());
        std::string message = std::vformat(translated, std::make_format_args(fmt...));
        std::cerr
        << fg::red(_("fatal"))
        << ": "
        << message
        << std::endl;
        std::exit(1);
    }
    
}; // END msg