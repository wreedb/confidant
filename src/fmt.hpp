// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <string_view>


namespace fmt {
    
    std::string bolden(std::string_view str);
    std::string ital(std::string_view str);
    std::string ul(std::string_view str);
    std::string sthru(std::string_view str);

    namespace fg {
        
        std::string black(std::string_view s);
        std::string red(std::string_view s);
        std::string green(std::string_view s);
        std::string yellow(std::string_view s);
        std::string blue(std::string_view s);
        std::string magenta(std::string_view s);
        std::string cyan(std::string_view s);
        std::string white(std::string_view s);
        
        std::string rgb(const int& red, const int& green, const int& blue);
    
    }; // END fmt::fg

    namespace bg {
        
        std::string black(std::string_view s);
        std::string red(std::string_view s);
        std::string green(std::string_view s);
        std::string yellow(std::string_view s);
        std::string blue(std::string_view s);
        std::string magenta(std::string_view s);
        std::string cyan(std::string_view s);
        std::string white(std::string_view s);
        
        std::string rgb(const int& red, const int& green, const int& blue);
    
    }; // END fmt::bg
    
}; // END fmt