// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "settings/global.hpp"
#include "settings/local.hpp"
#include "util.hpp"
#include <glob.h>
#include <type_traits>
#include <vector>
#include <variant>
#include <string>
#include <sstream>

#include "actions/get.hpp"

namespace confidant {
  
    namespace actions {
        
        namespace get {
            std::optional<globalvalue> global(const confidant::config::global::settings& conf, std::string_view qry) {
                auto parts = util::split(qry);
                if (parts.empty()) return std::nullopt;
                if (parts.at(0) == "create-directories")
                    return conf.createdirs;
                else if (parts.at(0) == "log-level")
                    return conf.loglevel;
                else if (parts.at(0) == "color")
                    return conf.color;
                else
                    return std::nullopt;
            }
            
            std::optional<localvalue> local(const confidant::config::local::settings& conf, std::string_view qry) {
                auto parts = util::split(qry);
                if (parts.empty()) return std::nullopt;
                if (parts.at(0) == "repository") {
                    if (parts.size() == 1) return conf.repo.url;
                    if (parts.size() == 2 && parts.at(1) == "url") return conf.repo.url;
                    return std::nullopt;
                }
                
                if (parts.at(0) == "links") {
                    if (parts.size() == 1) {
                        return conf.links;
                    }
                    if (parts.size() == 2) {
                        for (const auto& link : conf.links) 
                            if (link.name == parts.at(1)) return link;
                        return std::nullopt;
                    }
                    if (parts.size() == 3) {
                        for (const auto& link : conf.links) {
                            if (link.name == parts.at(1)) {
                                if (parts.at(2) == "source") return link.source.string();
                                if (parts.at(2) == "dest") return link.destination.string();
                                if (parts.at(2) == "type")
                                    return link.type == config::local::linktype::file ? "file" : "directory";
                                if (parts.at(2) == "tag" && !link.tag.empty())
                                    return link.tag;
                            }
                        }
                    }
                    return std::nullopt;
                }
                
                if (parts.at(0) == "templates") {
                    if (parts.size() == 1) return conf.templates;
                    if (parts.size() >= 2) {
                        for (const auto& tmpl : conf.templates) {
                            if (tmpl.name == parts.at(1)) {
                                if (parts.size() == 2) return tmpl;
                                
                                if (parts.size() == 3) {
                                    if (parts.at(2) == "source") return tmpl.source.string();
                                    if (parts.at(2) == "dest") return tmpl.destination.string();
                                    if (parts.at(2) == "items") return tmpl.items;
                                    if (parts.at(2) == "tag" && !tmpl.tag.empty())
                                        return tmpl.tag;
                                }
                            }
                        }
                    }
                    return std::nullopt;
                }
                return std::nullopt;
            }
            
            std::string formatglobalvalue(const globalvalue& v) {
                return std::visit([](auto&& arg) -> std::string {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, bool>)
                        return arg ? "true" : "false";
                    else if constexpr (std::is_same_v<T, std::string>)
                        return arg;
                    else if constexpr (std::is_same_v<T, util::verbose>)
                        return util::verboseliteral(arg);
                    else if constexpr (std::is_same_v<T, int>)
                        return std::to_string(arg);
                    return "unknown";
                }, v);
            }
            
            std::string formatlocalvalue(const localvalue& v) {
                return std::visit([](auto&& arg) -> std::string {
                    using T = std::decay_t<decltype(arg)>;
                    
                    if constexpr (std::is_same_v<T, bool>) {
                        return arg ? "true" : "false";
                    }
                    else if constexpr (std::is_same_v<T, std::string>) {
                        return arg;
                    }
                    else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
                        std::ostringstream oss;
                        oss << "[\n";
                        for (const auto& item : arg) {
                            oss << "  " << item << "\n";
                        }
                        oss << "]";
                        return oss.str();
                    }
                    else if constexpr (std::is_same_v<T, std::vector<config::local::link>>) {
                        std::ostringstream oss;
                        for (const auto& link : arg) {
                            oss << link.name << ":\n";
                            oss << "  source: " << link.source.string() << "\n";
                            oss << "  dest: " << link.destination.string() << "\n";
                            oss << "  type: " << (link.type == config::local::linktype::file ? "file" : "directory") << "\n";
                            if (!link.tag.empty()) oss << "  tag: " << link.tag << "\n";
                        }
                        return oss.str();
                    }
                    else if constexpr (std::is_same_v<T, config::local::link>) {
                        std::ostringstream oss;
                        oss << "source: " << arg.source.string() << "\n";
                        oss << "dest: " << arg.destination.string() << "\n";
                        if (!arg.tag.empty()) oss << "tag: " << arg.tag << "\n";
                        oss << "type: " << (arg.type == config::local::linktype::file ? "file" : "directory");
                        return oss.str();
                    }
                    else if constexpr (std::is_same_v<T, std::vector<config::local::templatelink>>) {
                        std::ostringstream oss;
                        for (const auto& tmpl : arg) {
                            oss << tmpl.name << ":\n";
                            oss << "  source: " << tmpl.source.string() << "\n";
                            oss << "  dest: " << tmpl.destination.string() << "\n";
                            if (!tmpl.tag.empty()) oss << "  tag: " << tmpl.tag << "\n";
                            oss << "  items: [" << tmpl.items.size() << " items]\n";
                        }
                        return oss.str();
                    }
                    else if constexpr (std::is_same_v<T, config::local::templatelink>) {
                        std::ostringstream oss;
                        oss << "source: " << arg.source.string() << "\n";
                        oss << "dest: " << arg.destination.string() << "\n";
                        if (!arg.tag.empty()) oss << "tag: " << arg.tag << "\n";
                        oss << "items:\n";
                        for (const auto& item : arg.items) {
                            oss << "  - " << item << "\n";
                        }
                        return oss.str();
                    }
                    
                    return "<unknown type>";
                }, v);
            }
        };
    
    };
    
};