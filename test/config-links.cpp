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
    std::string configpath = projectroot + "/test/aux/t/config-links/sample.ucl";
    confidant::settings globalConfig = options::global::defaults();
    confidant::configuration localConfig = confidant::config::serialize(configpath, globalConfig);
    confidant::debug::dumpConfig(localConfig);
    if (localConfig.links.size() != 4) {
        std::println(std::cerr, "[test] config-links: fail");
        std::println(std::cerr, "line 20: expected 4, got {}", localConfig.links.size());
        return 1;
    }
    if (localConfig.links.at(0).name != "foo") {
        std::println(std::cerr, "[test] config-links: fail");
        std::println(std::cerr, "line 25: expected 'foo', got '{}'", localConfig.links.at(0).name);
        return 1;
    }
    if (localConfig.links.at(1).name != "bar") {
        std::println(std::cerr, "[test] config-links: fail");
        std::println(std::cerr, "line 30: expected 'bar', got '{}'", localConfig.links.at(1).name);
        return 1;
    }
    if (localConfig.links.at(2).name != "fuzz") {
        std::println(std::cerr, "[test] config-links: fail");
        std::println(std::cerr, "line 35: expected 'fuzz', got '{}'", localConfig.links.at(2).name);
        return 1;
    }
    if (localConfig.links.at(3).name != "buzz") {
        std::println(std::cerr, "[test] config-links: fail");
        std::println(std::cerr, "line 40: expected 'buzz', got '{}'", localConfig.links.at(3).name);
        return 1;
    }
    std::println("[test] config-links: pass");
    return 0;
}