// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <vector>
#include <string>
#include <format>
#include <print>

#include "confidant.hpp"

using std::string;

namespace confidant {
namespace debug {
void dumpConfig(const confidant::configuration& conf) {
    std::println("confidant.ucl:");
    std::println("  repository: ");
    std::println("    url: {}", conf.repo.url);
    std::println("  link:");
    int numlinks = conf.links.size();
    for (int n = 0; n < numlinks; n++) {
        std::println("  - name: {}", conf.links.at(n).name);
        std::println("    source: {}", std::string(conf.links.at(n).name));
        std::println("    destination: {}", std::string(conf.links.at(n).destination));
        switch (conf.links.at(n).type) {
            case confidant::config::linkType::file:
                std::println("    type: file");
                break;
            case confidant::config::linkType::directory:
                std::println("    type: directory");
                break;
            // theoretically this should never be possible
            default:
                std::println("    type: unknown");
                break;
        }
    }
    
    std::println("  link-from:");
    std::println("    from: {}", std::string(conf.linkFrom.from));
    std::println("    to: {}", std::string(conf.linkFrom.to));
    std::println("    items:");
    int numitems = conf.linkFrom.items.size();
    for (int n = 0; n < numitems; n++) {
        std::println("    - {}", conf.linkFrom.items.at(n));
    }
}

} // NOTE: end namespace confidant::debug

} // NOTE: end namespace confidant