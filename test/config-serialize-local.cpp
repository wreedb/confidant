#include "settings/global.hpp"
#include "settings/local.hpp"

#include "test.hpp"

#include <iostream>
#include <print>
#include <format>

namespace config = confidant::config;

int main(const int argc, const char *argv[]) {
    
    std::string lpath = std::format("{}/test/aux/t/config-serialize-local/local.ucl", PROJECT_ROOT);
    std::string gpath = std::format("{}/test/aux/t/config-serialize-local/global.ucl", PROJECT_ROOT);

    config::global::settings gconf = config::global::serialize(gpath);
    config::local::settings conf = config::local::serialize(lpath, gconf);
    
    if (conf.links.at(0).tag.empty()) {
        std::println("pass");
    } else {
        std::println(std::cerr, "fail");
        return 1;
    }
    
    if (conf.templates.at(0).tag.empty()) {
        std::println("pass");
    } else {
        std::println(std::cerr, "fail");
        return 1;
    }
    
    if (!conf.links.at(1).tag.empty()) {
        std::println("pass: {}", conf.links.at(1).tag);
    } else {
        std::println(std::cerr, "fail");
        return 1;
    }
    
    if (!conf.templates.at(1).tag.empty()) {
        std::println("pass: {}", conf.templates.at(1).tag);
    } else {
        std::println(std::cerr, "fail");
        return 1;
    }
    
    return 0;
    
}