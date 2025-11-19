// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <sstream>
#include <optional>
#include <string_view>
#include <variant>
#include "util.hpp"
#include "ucl.hpp"
#include "logging.hpp"

namespace confidant {

std::string substitute(const std::string& tmpl, const std::string& item) {
    const std::string placeholder = "%{item}";
    std::string result = tmpl;
    
    size_t pos = 0;
    while ((pos = result.find(placeholder, pos)) != std::string::npos) {
        result.replace(pos, placeholder.length(), item);
        pos += item.length();
    }
    return result;
}

namespace config {
    
    using ConfigValue = std::variant<
        bool,
        string,
        vector<string>,
        vector<confidant::config::link>,
        vector<confidant::config::templatelink>,
        confidant::config::link,
        confidant::config::templatelink
    >;
    
    std::optional<ConfigValue> get(const confidant::configuration& conf, string_view qry) {
        auto parts = util::split(qry);
        if (parts.empty()) return std::nullopt;
        if (parts.at(0) == "create-directories") {
            if (parts.size() == 1) return conf.create_dirs;
            return std::nullopt;
        }
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
                            return link.type == linkType::file ? "file" : "directory";
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
                        }
                    }
                }
            }
            return std::nullopt;
        }
        return std::nullopt;
    }
    std::string formatconfigvalue(const ConfigValue& value) {
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
            else if constexpr (std::is_same_v<T, std::vector<config::link>>) {
                std::ostringstream oss;
                for (const auto& link : arg) {
                    oss << link.name << ":\n";
                    oss << "  source: " << link.source.string() << "\n";
                    oss << "  dest: " << link.destination.string() << "\n";
                    oss << "  type: " << (link.type == config::linkType::file ? "file" : "directory") << "\n";
                }
                return oss.str();
            }
            else if constexpr (std::is_same_v<T, config::link>) {
                std::ostringstream oss;
                oss << "source: " << arg.source.string() << "\n";
                oss << "dest: " << arg.destination.string() << "\n";
                oss << "type: " << (arg.type == config::linkType::file ? "file" : "directory");
                return oss.str();
            }
            else if constexpr (std::is_same_v<T, std::vector<config::templatelink>>) {
                std::ostringstream oss;
                for (const auto& tmpl : arg) {
                    oss << tmpl.name << ":\n";
                    oss << "  source: " << tmpl.source.string() << "\n";
                    oss << "  dest: " << tmpl.destination.string() << "\n";
                    oss << "  items: [" << tmpl.items.size() << " items]\n";
                }
                return oss.str();
            }
            else if constexpr (std::is_same_v<T, config::templatelink>) {
                std::ostringstream oss;
                oss << "source: " << arg.source.string() << "\n";
                oss << "dest: " << arg.destination.string() << "\n";
                oss << "items:\n";
                for (const auto& item : arg.items) {
                    oss << "  - " << item << "\n";
                }
                return oss.str();
            }
            
            return "<unknown type>";
        }, value);
    }
    
}; // END confidant::config
}; // END confidant