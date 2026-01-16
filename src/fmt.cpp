// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>
#include <string_view>
#include <format>
#include <algorithm>

#include "options.hpp"
#include "settings/global.hpp"
#include "ansi.hpp"
#include "fmt.hpp"

using std::format;
using std::string;
using sview = std::string_view;

namespace fmt {
    
    string bolden(sview str) {
        if (confidant::config::global::color && options::global::color)
            return format("{}{}{}", ansi::bold, str, ansi::freset);
        else
            return string(str);
    }
    
    string ital(sview str) {
        if (confidant::config::global::color && options::global::color)
            return format("{}{}{}", ansi::italic, str, ansi::freset);
        else return string(str);
    }
    
    string ul(sview str) {
        if (confidant::config::global::color && options::global::color)
            return format("{}{}{}", ansi::underline, str, ansi::freset);
        else return string(str);
    }
    
    string sthru(sview str) {
        if (confidant::config::global::color && options::global::color)
            return format("{}{}{}", ansi::strikethru, str, ansi::freset);
        else return string(str);
    }
    
    namespace fg {
        string black(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::black, s, ansi::fg::reset);
            else return string(s);
        }
    
        string red(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::red, s, ansi::fg::reset);
            else return string(s);
        }
    
        string green(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::green, s, ansi::fg::reset);
            else return string(s);
        }
    
        string yellow(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::yellow, s, ansi::fg::reset);
            else return string(s);
        }
    
        string blue(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::blue, s, ansi::fg::reset);
            else return string(s);
        }
    
        string magenta(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::magenta, s, ansi::fg::reset);
            else return string(s);
        }
    
        string cyan(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::cyan, s, ansi::fg::reset);
            else return string(s);
        }
    
        string white(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::fg::white, s, ansi::fg::reset);
            else return string(s);
        }
        string rgb(int red, int green, int blue) {
            if (confidant::config::global::color && options::global::color)
                return format(
                "\033[38;2;{};{};{}m",
                std::clamp(red,   0, 255),
                std::clamp(green, 0, 255),
                std::clamp(blue,  0, 255));
            else return "";
        }
    }; // END fg
    
    namespace bg {
        string black(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::black, s, ansi::fg::reset);
            else return string(s);
        }
    
        string red(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::red, s, ansi::fg::reset);
            else return string(s);
        }
    
        string green(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::green, s, ansi::fg::reset);
            else return string(s);
        }
    
        string yellow(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::yellow, s, ansi::fg::reset);
            else return string(s);
        }
    
        string blue(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::blue, s, ansi::fg::reset);
            else return string(s);
        }
    
        string magenta(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::magenta, s, ansi::fg::reset);
            else return string(s);
        }
    
        string cyan(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::cyan, s, ansi::fg::reset);
            else return string(s);
        }
    
        string white(sview s) {
            if (confidant::config::global::color && options::global::color)
                return format("{}{}{}", ansi::bg::white, s, ansi::fg::reset);
            else return string(s);
        }
        string rgb(int red, int green, int blue) {
            if (confidant::config::global::color && options::global::color)
                return format(
                "\033[48;2;{};{};{}m",
                std::clamp(red,   0, 255),
                std::clamp(green, 0, 255),
                std::clamp(blue,  0, 255));
            else return "";
        }
    }; // END bg
    
}; // END fmt
