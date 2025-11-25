// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>
#include <string_view>
#include <format>
#include <algorithm>

#include "options.hpp"
#include "settings/global.hpp"
#include "ansi.hpp"
#include "fmt.hpp"

namespace fmt {
    
    std::string bolden(std::string_view str) {
        if (confidant::config::global::color && options::global::color)
            return std::format("{}{}{}", ansi::bold, str, ansi::freset);
        else
            return std::string(str);
    }
    
    std::string ital(std::string_view str) {
        if (confidant::config::global::color && options::global::color)
            return std::format("{}{}{}", ansi::italic, str, ansi::freset);
        else return std::string(str);
    }
    
    std::string ul(std::string_view str) {
        if (confidant::config::global::color && options::global::color)
            return std::format("{}{}{}", ansi::underline, str, ansi::freset);
        else return std::string(str);
    }
    
    std::string sthru(std::string_view str) {
        if (confidant::config::global::color && options::global::color)
            return std::format("{}{}{}", ansi::strikethru, str, ansi::freset);
        else return std::string(str);
    }
    
    namespace fg {
        std::string black(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::fg::black, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string red(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::fg::red, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string green(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::fg::green, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string yellow(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::yellow, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string blue(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::fg::blue, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string magenta(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::fg::magenta, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string cyan(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::fg::cyan, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string white(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::fg::white, s, ansi::fg::reset);
            else return std::string(s);
        }
        std::string rgb(int red, int green, int blue) {
            if (confidant::config::global::color && options::global::color)
                return std::format(
                "\033[38;2;{};{};{}m",
                std::clamp(red,   0, 255),
                std::clamp(green, 0, 255),
                std::clamp(blue,  0, 255));
            else return "";
        }
    }
    
    namespace bg {
        std::string black(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::bg::black, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string red(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::bg::red, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string green(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::bg::green, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string yellow(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::yellow, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string blue(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::bg::blue, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string magenta(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::bg::magenta, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string cyan(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::bg::cyan, s, ansi::fg::reset);
            else return std::string(s);
        }
    
        std::string white(std::string_view s) {
            if (confidant::config::global::color && options::global::color)
                return std::format("{}{}{}", ansi::bg::white, s, ansi::fg::reset);
            else return std::string(s);
        }
        std::string rgb(int red, int green, int blue) {
            if (confidant::config::global::color && options::global::color)
                return std::format(
                "\033[48;2;{};{};{}m",
                std::clamp(red,   0, 255),
                std::clamp(green, 0, 255),
                std::clamp(blue,  0, 255));
            else return "";
        }
    }
    
}