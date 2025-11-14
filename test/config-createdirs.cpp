#include <print>
#include <iostream>
#include "confidant.hpp"
#include "help.hpp"
#include "util.hpp"
#include "ucl.hpp"
#include "logging.hpp"
#include "xdg.hpp"
#include "options.hpp"

#include "config.hpp"
#include "test.hpp"

static const std::string projectroot = PROJECT_ROOT;

int main() {
    std::string local_config_path = projectroot + "/test/aux/t/config-createdirs/local.ucl";
    std::string global_config_path = projectroot + "/test/aux/t/config-createdirs/global.ucl";
    auto globalConfig = confidant::config::global::serialize(global_config_path);
    auto localConfig = confidant::config::serialize(local_config_path, globalConfig);
    if (localConfig.create_dirs != false) {
        std::println(std::cerr, "[test] config-createdirs: fail");
        std::println(std::cerr, "line 20: expected true, got false");
        return 1;
    }
    if (globalConfig.create_dirs != true) {
        std::println(std::cerr, "[test] config-createdirs: fail");
        std::println(std::cerr, "line 24: expected false, got true");
        return 1;
    }
    std::println("[test] config-createdirs: pass");
    return 0;
}