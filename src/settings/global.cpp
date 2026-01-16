// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "parse.hpp"
#include "util.hpp"
#include "settings/global.hpp"

namespace confidant {
    namespace config {
        namespace global {
            
            confidant::config::global::settings serialize(std::string_view path) {
                
                // default values are set already
                confidant::config::global::settings config;
                
                ucl::Ucl input = ucl::parsing::file(path, std::map<std::string, std::string>{});
                
                if (ucl::check(input, "create-directories")) {
                    auto n = ucl::get::boolean(input, "create-directories");
                    if (n) config.createdirs = n.value();
                }
                
                if (ucl::check(input, "color")) {
                    auto n = ucl::get::boolean(input, "color");
                    if (n) config.color = n.value();
                }
                
                if (ucl::check(input, "log-level")) {
                    if (input["log-level"].type() == ucl::Int) {
                        auto n = ucl::get::integer(input, "log-level");
                        if (n) {
                            switch (n.value()) {
                                case 0:
                                    config.loglevel = util::verbose::quiet;
                                    break;
                                case 1:
                                    config.loglevel = util::verbose::normal;
                                    break;
                                case 2:
                                    config.loglevel = util::verbose::info;
                                    break;
                                case 3:
                                    config.loglevel = util::verbose::debug;
                                    break;
                                case 4:
                                    config.loglevel = util::verbose::trace;
                                    break;
                                default:
                                    config.loglevel = util::verbose::normal;
                                    break;
                            }
                        }
                    } else if (input["log-level"].type() == ucl::String) {
                        auto n = ucl::get::str(input, "log-level");
                        if (n) {
                            std::string v = n.value();
                            if (v == "quiet") config.loglevel = util::verbose::quiet;
                            else if (v == "normal") config.loglevel = util::verbose::normal;
                            else if (v == "info") config.loglevel = util::verbose::info;
                            else if (v == "debug") config.loglevel = util::verbose::debug;
                            else if (v == "trace") config.loglevel = util::verbose::trace;
                            else config.loglevel = util::verbose::normal;
                        }
                    }
                }
                
                return config;
            }
            
        }; // END global

    }; // END config
    
}; // END confidant
