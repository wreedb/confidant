// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>
#include <format>
#include <algorithm>

#include "logging.hpp"
#include "config.hpp"

using std::string;
using std::string_view;

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
        
        
        std::string bolden(std::string_view str) {
            if (!ansi::useColor)
                return std::string(str);
            else
                return std::format("{}{}{}", ansi::bold, str, ansi::freset);
        }
        
        std::string ital(std::string_view str) {
            if (!ansi::useColor)
                return std::string(str);
            else
                return std::format("{}{}{}", ansi::italic, str, ansi::freset);
        }
        
        std::string ul(std::string_view str) {
            if (!ansi::useColor)
                return std::string(str);
            else
                return std::format("{}{}{}", ansi::underline, str, ansi::freset);
        }
        
        std::string sthru(std::string_view str) {
            if (!ansi::useColor)
                return std::string(str);
            else
                return std::format("{}{}{}", ansi::strikethru, str, ansi::freset);
        }
        
        std::string color(const int& num) {
            if (!ansi::useColor)
                return "";
            else {
                return "\033[" + std::to_string(num) + "m";
            }
        }
        
        namespace fg {
            string black(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::fg::black, s, ansi::fg::reset);
                else
                    return string(s);
            }
        
            string red(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::fg::red, s, ansi::fg::reset);
                else
                    return string(s);
            }
        
            string green(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::fg::green, s, ansi::fg::reset);
                else
                    return string(s);
            }
        
            string yellow(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::fg::yellow, s, ansi::fg::reset);
                else
                    return string(s);
            }
        
            string blue(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::fg::blue, s, ansi::fg::reset);
                else
                    return string(s);
            }
        
            string magenta(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::fg::magenta, s, ansi::fg::reset);
                else
                    return string(s);
            }
        
            string cyan(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::fg::cyan, s, ansi::fg::reset);
                else
                    return string(s);
            }
        
            string white(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::fg::white, s, ansi::fg::reset);
                else
                    return string(s);
            }
        }; // end confidant::logging::fg
        
        namespace bg {
            string black(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::bg::black, s, ansi::bg::reset);
                else
                    return string(s);
            }
        
            string red(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::bg::red, s, ansi::bg::reset);
                else
                    return string(s);
            }
        
            string green(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::bg::green, s, ansi::bg::reset);
                else
                    return string(s);
            }
        
            string yellow(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::bg::yellow, s, ansi::bg::reset);
                else
                    return string(s);
            }
        
            string blue(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::bg::blue, s, ansi::bg::reset);
                else
                    return string(s);
            }
        
            string magenta(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::bg::magenta, s, ansi::bg::reset);
                else
                    return string(s);
            }
        
            string cyan(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::bg::cyan, s, ansi::bg::reset);
                else
                    return string(s);
            }
        
            string white(string_view s) {
                if (ansi::useColor)
                    return format("{}{}{}", ansi::bg::white, s, ansi::bg::reset);
                else
                    return string(s);
            }
        }; // END confidant::logging::bg
        
    }
}