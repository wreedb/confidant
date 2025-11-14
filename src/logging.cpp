// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>
#include <format>
#include <algorithm>

#include "logging.hpp"
#include "config.hpp"

namespace logger = confidant::logging;

namespace ansi {
std::string verbosity_literal(const int& v) {
    using ansi::verbosity;
    switch (v) {
        case verbosity::quiet:
            return "quiet";
        case verbosity::normal:
            return "normal";
        case verbosity::info:
            return "info";
        case verbosity::debug:
            return "debug";
        case verbosity::trace:
            return "trace";
        default:
            logger::fatal(PROJECT_NAME, 1, "log-level {} is invalid!", v);
    }
    
}

namespace fg {
std::string rgb(const int& red, const int& green, const int& blue) {
    return std::format("\033[38;2;{};{};{}m",
    std::clamp(red,   0, 255),
    std::clamp(green, 0, 255),
    std::clamp(blue,  0, 255));
}
}

namespace bg {
std::string rgb(const int& red, const int& green, const int& blue) {
    return std::format("\033[48;2;{};{};{}m",
    std::clamp(red,   0, 255),
    std::clamp(green, 0, 255),
    std::clamp(blue,  0, 255));
}
}
}

namespace confidant {
    namespace logging {
        std::string color(const int& num) {
            if (!ansi::useColor)
                return std::string("");
            else
                return std::format("\033[{}m", num);
        }
    }
}