
#include <string>
#include <string_view>
#include <format>
#include <algorithm>

#include "settings/global.hpp"
#include "ansi.hpp"
#include "fmt.hpp"

namespace fmt {
    
    std::string bolden(std::string_view str) {
        if (!confidant::config::global::color)
            return std::string(str);
        else
            return std::format("{}{}{}", ansi::bold, str, ansi::freset);
    }
    
    std::string ital(std::string_view str) {
        if (!confidant::config::global::color)
            return std::string(str);
        else
            return std::format("{}{}{}", ansi::italic, str, ansi::freset);
    }
    
    std::string ul(std::string_view str) {
        if (!confidant::config::global::color)
            return std::string(str);
        else
            return std::format("{}{}{}", ansi::underline, str, ansi::freset);
    }
    
    std::string sthru(std::string_view str) {
        if (!confidant::config::global::color)
            return std::string(str);
        else
            return std::format("{}{}{}", ansi::strikethru, str, ansi::freset);
    }
    
    namespace fg {
        std::string black(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::fg::black, s, ansi::fg::reset);
            else
                return std::string(s);
        }
    
        std::string red(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::fg::red, s, ansi::fg::reset);
            else
                return std::string(s);
        }
    
        std::string green(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::fg::green, s, ansi::fg::reset);
            else
                return std::string(s);
        }
    
        std::string yellow(std::string_view s) {
            if (confidant::config::global::color)
                return format("{}{}{}", ansi::fg::yellow, s, ansi::fg::reset);
            else
                return std::string(s);
        }
    
        std::string blue(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::fg::blue, s, ansi::fg::reset);
            else
                return std::string(s);
        }
    
        std::string magenta(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::fg::magenta, s, ansi::fg::reset);
            else
                return std::string(s);
        }
    
        std::string cyan(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::fg::cyan, s, ansi::fg::reset);
            else
                return std::string(s);
        }
    
        std::string white(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::fg::white, s, ansi::fg::reset);
            else
                return std::string(s);
        }
        std::string rgb(const int& red, const int& green, const int& blue) {
            if (!confidant::config::global::color) return "";
            else return std::format(
                "\033[38;2;{};{};{}m",
                std::clamp(red,   0, 255),
                std::clamp(green, 0, 255),
                std::clamp(blue,  0, 255));
        }
    }
    
    namespace bg {
        std::string black(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::bg::black, s, ansi::bg::reset);
            else
                return std::string(s);
        }
    
        std::string red(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::bg::red, s, ansi::bg::reset);
            else
                return std::string(s);
        }
    
        std::string green(std::string_view s) {
            if (confidant::config::global::color)
                return format("{}{}{}", ansi::bg::green, s, ansi::bg::reset);
            else
                return std::string(s);
        }
    
        std::string yellow(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::bg::yellow, s, ansi::bg::reset);
            else
                return std::string(s);
        }
    
        std::string blue(std::string_view s) {
            if (confidant::config::global::color)
                return format("{}{}{}", ansi::bg::blue, s, ansi::bg::reset);
            else
                return std::string(s);
        }
    
        std::string magenta(std::string_view s) {
            if (confidant::config::global::color)
                return format("{}{}{}", ansi::bg::magenta, s, ansi::bg::reset);
            else
                return std::string(s);
        }
    
        std::string cyan(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::bg::cyan, s, ansi::bg::reset);
            else
                return std::string(s);
        }
    
        std::string white(std::string_view s) {
            if (confidant::config::global::color)
                return std::format("{}{}{}", ansi::bg::white, s, ansi::bg::reset);
            else
                return std::string(s);
        }
        
        std::string rgb(const int& red, const int& green, const int& blue) {
            if (!confidant::config::global::color) return "";
            else return std::format(
                "\033[48;2;{};{};{}m",
                std::clamp(red,   0, 255),
                std::clamp(green, 0, 255),
                std::clamp(blue,  0, 255));
        }
    }
    
}