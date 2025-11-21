#include "settings/global.hpp"

#include "util.hpp"
#include "test.hpp"

#include <iostream>
#include <print>
#include <format>

namespace config = confidant::config;

int main(const int argc, const char *argv[]) {
    
    std::string gpath = std::format("{}/test/aux/t/config-serialize-global/global.ucl", PROJECT_ROOT);

    config::global::settings gconf = config::global::serialize(gpath);
  
    if (!gconf.color) return 1;
    if (!gconf.createdirs) return 1;
    if (gconf.loglevel != util::verbose::trace) return 1;
    return 0;
    
}