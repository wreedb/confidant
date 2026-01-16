// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <string>
#include <string_view>

using std::string;
using sview = std::string_view;

namespace fmt {
    
    string bolden(sview str);
    string ital(sview str);
    string ul(sview str);
    string sthru(sview str);

    namespace fg {
        
        string black(sview s);
        string red(sview s);
        string green(sview s);
        string yellow(sview s);
        string blue(sview s);
        string magenta(sview s);
        string cyan(sview s);
        string white(sview s);
        
        string rgb(int red, int green, int blue);
    
    }; // END fg

    namespace bg {
        
        string black(sview s);
        string red(sview s);
        string green(sview s);
        string yellow(sview s);
        string blue(sview s);
        string magenta(sview s);
        string cyan(sview s);
        string white(sview s);
        
        string rgb(int red, int green, int blue);
    
    }; // END bg
    
}; // END fmt
