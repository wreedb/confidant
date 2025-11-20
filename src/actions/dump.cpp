// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <utility>
#include <print>
#include "settings/local.hpp"
#include "settings/global.hpp"

#include "util.hpp"
#include "msg.hpp"
#include "fmt.hpp"

namespace confidant {
    namespace actions {
        namespace dump {

            void global(const confidant::config::global::settings& conf) {
                msg::pretty("global configuration");
                std::println("{}: {}", fmt::fg::blue("create-directories"), conf.createdirs);
                std::println("{}: {}", fmt::fg::blue("log-level"), util::verboseliteral(conf.loglevel));
                std::println("{}: {}", fmt::fg::blue("color"), conf.color);
            }

    
            void local(const confidant::config::local::settings& conf) {
                std::println("{}:", fmt::fg::blue("repository"));
                std::println("  {}: {}", fmt::fg::blue("url"), conf.repo.url);
                int numlinks = conf.links.size();
                int numtemplates = conf.templates.size();
                if (numlinks > 0) {
                    std::println("{}:", fmt::fg::blue("links"));
                    for (int n = 0; n < numlinks; n++) {
                        std::println("- {}: {}", fmt::fg::blue("name"), conf.links.at(n).name);
                        std::println("  {}: {}", fmt::fg::blue("source"), conf.links.at(n).source.string());
                        std::println("  {}: {}", fmt::fg::blue("destination"), conf.links.at(n).destination.string());
                        switch (conf.links.at(n).type) {
                            case config::local::linktype::file:
                                std::println("  {}: file", fmt::fg::blue("type"));
                                break;
                            case config::local::linktype::directory:
                                std::println("  {}: directory", fmt::fg::blue("type"));
                                break;
                            default:
                                // invalid or absent values will have been replaced with 'file'
                                std::unreachable();
                        }
                    }
                }
                
                if (numtemplates > 0) {
                    std::println("{}:", fmt::fg::blue("templates"));
                    for (int n = 0; n < numtemplates; n++) {
                        std::string name = conf.templates.at(n).name;
                        std::string src = conf.templates.at(n).source.string();
                        std::string dst = conf.templates.at(n).destination.string();
                        std::println("- {}: {}", fmt::fg::blue("name"), name);
                        std::println("  {}: {}", fmt::fg::blue("source"), src);
                        std::println("  {}: {}", fmt::fg::blue("destination"), dst);
                        int numitems = conf.templates.at(n).items.size();
                        if (numitems > 0) {
                            std::println("  {}:", fmt::fg::blue("items"));
                            for (int x = 0; x < numitems; x++) {
                                std::println("  - {}", fmt::fg::green(conf.templates.at(n).items.at(x)));
                            }
                        }
                    }
                }
            }
        } // END confidant::actions::dump
    } // END confidant::actions
} // END confidant