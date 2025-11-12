#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include "util.hpp"


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

inline bool useColor = util::hasenv("NO_COLOR") ? false : true;
inline int loglevel = ansi::verbosity::normal;

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
}

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
}

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
}
}

namespace confidant {
namespace logging {
std::string color(const int& num);
template <typename... Args>
void err(std::string_view argz, std::string_view msg, Args&&... fmt) {
    std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
    std::cerr
    << logging::color(31)
    << argz
    << logging::color(0)
    << ": "
    << fmtMsg
    << "\n";
}

template <typename... Args>
void info (std::string_view argz, std::string_view msg, Args&&... fmt) {
    std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
    std::cout
    << logging::color(34)
    << argz
    << logging::color(0)
    << ": "
    << fmtMsg
    << "\n";
}
template <typename... Args>
void warn (std::string_view argz, std::string_view msg, Args&&... fmt) {
    if (ansi::loglevel < ansi::verbosity::info) return;
    std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
    std::cout
    << logging::color(33)
    << argz
    << logging::color(0)
    << ": "
    << fmtMsg
    << std::endl;
}

template <typename... Args>
[[noreturn]] void fatal(std::string_view argz, int exitCode, std::string_view msg, Args&&... fmt) {
    std::string fmtMsg = std::vformat(msg.data(), std::make_format_args(fmt...));
    std::cerr
    << logging::color(31)
    << argz
    << logging::color(0)
    << " (fatal): "
    << fmtMsg
    << std::endl;
    std::exit(exitCode);
}
}
}