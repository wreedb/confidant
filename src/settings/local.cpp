// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <string>
#include <map>
#include "xdg.hpp"
#include "util.hpp"
#include "parse.hpp"

#include "fmt.hpp"
#include "msg.hpp"

#include "settings/local.hpp"

namespace fs = std::filesystem;

namespace confidant {
    
    namespace config {
        
        namespace local {
            
            confidant::config::local::settings serialize(std::string_view path, const confidant::config::global::settings& globals) {
                confidant::config::local::settings conf;
             
                std::map<std::string, std::string> vars;
         
                // get some xdg variables and add them to vars
                std::map<std::string, fs::path> xdghomes = xdg::homes();
                vars = ucl::var::add("XDG_CONFIG_HOME", xdghomes["XDG_CONFIG_HOME"].string(), vars);
                vars = ucl::var::add("XDG_CACHE_HOME",  xdghomes["XDG_CACHE_HOME"].string(),  vars);
                vars = ucl::var::add("XDG_DATA_HOME",   xdghomes["XDG_DATA_HOME"].string(),   vars);
                vars = ucl::var::add("XDG_STATE_HOME",  xdghomes["XDG_STATE_HOME"].string(),  vars);
                
                vars = ucl::var::add("xdg_config_home", xdghomes["XDG_CONFIG_HOME"].string(), vars);
                vars = ucl::var::add("xdg_cache_home",  xdghomes["XDG_CACHE_HOME"].string(),  vars);
                vars = ucl::var::add("xdg_data_home",   xdghomes["XDG_DATA_HOME"].string(),   vars);
                vars = ucl::var::add("xdg_state_home",  xdghomes["XDG_STATE_HOME"].string(),  vars);
                
                vars = ucl::var::add("XDG_RUNTIME_DIR", xdghomes["XDG_RUNTIME_DIR"].string(), vars);
                vars = ucl::var::add("xdg_runtime_dir", xdghomes["XDG_RUNTIME_DIR"].string(), vars);
         
                std::optional<std::string> homedir = util::getenv("HOME");
                if (!homedir)
                    throw std::runtime_error("HOME is not set in the environment!");
             
                vars = ucl::var::add("HOME", homedir.value(), vars);
                vars = ucl::var::add("home", homedir.value(), vars);
                vars = ucl::var::add("REPO", fs::path(path).parent_path().string(), vars);
                vars = ucl::var::add("repo", fs::path(path).parent_path().string(), vars);
                
                std::optional<std::string> username = util::getenv("USER");
                if (username) {
                    vars = ucl::var::add("user", username.value(), vars);
                    vars = ucl::var::add("USER", username.value(), vars);
                }
                
                std::optional<std::string> email = util::getenv("EMAIL");
                if (email) {
                    vars = ucl::var::add("email", email.value(), vars);
                    vars = ucl::var::add("EMAIL", email.value(), vars);
                }
                
                ucl::Ucl input = ucl::parsing::file(path, vars);
                
                // BEGIN serializing
                
                // BEGEIN repository
                if (ucl::check(input, "repository")) {
                    if (ucl::check(input["repository"], "url")) {
                        auto n = ucl::get::str(input["repository"], "url");
                        if (!n) msg::fatal("failed to parse {} value as a string!", fmt::bolden("repository.url"));
                        conf.repo.url = n.value();
                    }
                } else {
                    conf.repo.url = "";
                }
                // END repository
                
                // BEGIN links
                if (!ucl::check(input, "links")) {
                    msg::debug("field {} not specified", fmt::bolden("links"));
                } else {
                    auto oobj = ucl::get::node(input, "links");
                    if (!oobj) {
                        msg::fatal("failed to parse {} as a node!", fmt::bolden("links"));
                    } else {
                        ucl::Ucl obj = oobj.value();
                        int numlinks = ucl::members(obj);
                        conf.links.reserve(numlinks);
                        
                        // iterate over links
                        for (const auto& n : obj) {
                            confidant::config::local::link link;
                            
                            link.name = n.key();
                            
                            // BEGIN tag
                            if (ucl::check(n, "tag") && n["tag"].type() == ucl::String) {
                                auto s = ucl::get::str(n, "tag");
                                if (!s)
                                    msg::fatal("failed to parse {} field {} as a string!", fmt::bolden(link.name),
                                                                                           fmt::bolden("tag"));
                                else
                                    link.tag = s.value();                                
                            }
                            // END tag
                            
                            // BEGIN source
                            if (ucl::check(n, "source") && n["source"].type() == ucl::String) {
                                auto s = ucl::get::str(n, "source");
                                if (!s) msg::fatal("failed to get {} field {} as a string!", fmt::bolden(link.name), fmt::bolden("source"));
                                else link.source = fs::path(s.value());
                            } else if (ucl::check(n, "source") &&  n["source"].type() != ucl::String) {
                                msg::fatal("{} field {} must be a string!", fmt::bolden(link.name), fmt::bolden("source"));
                            } else {
                                msg::fatal("link {} is missing a {} value!", fmt::bolden(link.name), fmt::bolden("source"));
                            }
                            // END source

                            // BEGIN dest/destdir
                            if (ucl::check(n, "dest")) {
                                auto s = ucl::get::str(n, "dest");
                                if (!s) msg::fatal("failed to parse {} field {} as a string!", fmt::bolden(link.name), fmt::bolden("dest"));
                                else link.destination = fs::path(s.value());
                            } else if (ucl::check(n, "destdir")) {
                                auto s  = ucl::get::str(n, "destdir");
                                if (!s) msg::fatal("failed to parse {} field {} as a string!", fmt::bolden(link.name), fmt::bolden("destdir"));
                                std::string bn = link.source.filename().string();    
                                link.destination = fs::path(std::format("{}/{}", s.value(), bn));
                            } else {
                                msg::fatal(1, "link {} is missing a {} value!", fmt::bolden(link.name), fmt::bolden("dest/destdir"));
                            }
                            // END dest/destdir
                            
                            // BEGIN type
                            if (!ucl::check(n, "type")) {
                                link.type = linktype::file;
                            } else {
                                auto t = ucl::get::str(n, "type");
                                if (!t) msg::fatal("failed to parse {} field {} as a string!", fmt::bolden(link.name), fmt::bolden("type"));
                                else if (t.value() == "file") link.type = linktype::file;
                                else if (t.value() == "directory") link.type = linktype::directory;
                                else {
                                    msg::warn("type {} is not recognized, expected one of {} or {}, using default",
                                        fmt::ital(t.value()),
                                        fmt::bolden("file"),
                                        fmt::bolden("directory"));
                                    
                                    link.type = linktype::file;
                                }
                            }
                            // END type
                            conf.links.push_back(link);
                        }
                    }
                }
                // END links
                // BEGIN templates
                if (!ucl::check(input, "templates")) {
                    msg::debug("field {} not specified", fmt::bolden("templates"));
                } else {
                    auto oobj = ucl::get::node(input, "templates");
                    if (!oobj) {
                        msg::fatal("failed to parse {} as a node!", fmt::bolden("templates"));
                    } else {
                        ucl::Ucl obj = oobj.value();
                        int numtemplates = ucl::members(obj);
                        conf.templates.reserve(numtemplates);
                        
                        // iterate over templates
                        for (const auto& tmpl : obj) {
                            confidant::config::local::templatelink t;
                            
                            t.name = tmpl.key();
                            
                            // optional condition tag
                            if (ucl::check(tmpl, "tag") && tmpl["tag"].type() == ucl::String) {
                                auto s = ucl::get::str(tmpl, "tag");
                                if (!s)
                                    msg::fatal("failed to parse {} field {} as a string!", fmt::bolden(t.name),
                                                                                           fmt::bolden("tag"));
                                else
                                    t.tag = s.value();                                
                            }
                            
                            if (ucl::check(tmpl, "source")) {
                                t.source = fs::path(tmpl["source"].string_value());
                            
                            } else {
                                msg::fatal("template {} is missing a {} value!",
                                    fmt::bolden(t.name),
                                    fmt::ital("source"));
                                
                            }
                                
                            if (ucl::check(tmpl, "dest")) {
                                t.destination = fs::path(tmpl["dest"].string_value());
                            
                            } else {
                                msg::fatal("template {} is missing a {} value!",
                                    fmt::bolden(t.name),
                                    fmt::ital("dest"));
                            }
                            
                            if (ucl::check(tmpl, "items") && tmpl["items"].type() == ucl::Array) {
                                // items field exists and is an array (normal, correct)
                                int numitems = tmpl["items"].size();
                                t.items.reserve(numitems);
                                
                                // iterate over items
                                for (int i = 0; i < numitems; i++) {
                                    auto lst = ucl::get::list(tmpl, "items");
                                    if (!lst) {
                                        msg::fatal("failed to get {} items field as a list!",
                                            fmt::bolden(t.name));
                                    } else {
                                        t.items.push_back(lst.value().at(i).string_value());
                                    }
                                }
                                
                            } else if (ucl::check(tmpl, "items") && tmpl["items"].type() != ucl::Array) {
                                // items field exists, but is not an array (fatal)
                                msg::fatal("template {} {} field is not a list!",
                                    fmt::bolden(t.name),
                                    fmt::ital("items"));
                            } else {
                                // items field doesn't exist (fatal)
                                msg::fatal("template {} has no {} list!",
                                    fmt::bolden(t.name),
                                    fmt::ital("items"));
                            }
                            conf.templates.push_back(t);
                            
                        }
                    }
                }
                // END templates
                
                // return configuration
                return conf;
            }

            
        };
    };
    
};