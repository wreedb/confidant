// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "settings.hpp"
#include "settings/global.hpp"
#include "parse.hpp"
#include "logging.hpp"
#include "xdg.hpp"

#include <string_view>

namespace say = confidant::logging;

namespace confidant {
    namespace config {
        namespace local {
            confidant::configuration serialize(std::string_view path, const confidant::settings& globals) {
                confidant::configuration conf;
             
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
                
                // BEGIN create-directories
                if (ucl::check(input, "create-directories")) {
                    if (input["create-directories"].type() == ucl::Bool) {
                        auto n = ucl::get::boolean(input, "create-directories");
                        if (!n) say::fatal(1, "failed to parse {} as a boolean!", say::bolden("create-directories"));
                        conf.create_dirs = n.value();
                    } else {
                        say::error("create-directories value is invalid\nexpected one of: true (default), false, yes, no, on, off; using global setting {}", globals.createdirs);
                        conf.create_dirs = globals.createdirs;
                    }
                } else {
                    conf.create_dirs = globals.createdirs;
                }
                // END create-directories
                
                // BEGEIN repository
                if (ucl::check(input, "repository")) {
                    if (ucl::check(input["repository"], "url")) {
                        auto n = ucl::get::str(input["repository"], "url");
                        if (!n) say::fatal(1, "failed to parse {} value as a string!", say::bolden("repository.url"));
                        conf.repo.url = n.value();
                    }
                } else {
                    conf.repo.url = "";
                }
                // END repository
                
                // BEGIN links
                if (!ucl::check(input, "links")) {
                    say::warnextra("field {} not specified", say::bolden("links"));
                } else {
                    auto oobj = ucl::get::node(input, "links");
                    if (!oobj) {
                        say::fatal(1, "failed to parse {} as a node!", say::bolden("links"));
                    } else {
                        ucl::Ucl obj = oobj.value();
                        int numlinks = ucl::members(obj);
                        conf.links.reserve(numlinks);
                        
                        for (const auto& n : obj) {
                            confidant::config::link link;
                            
                            link.name = n.key();
                            
                            // BEGIN source
                            if (ucl::check(n, "source") && n["source"].type() == ucl::String) {
                                auto s = ucl::get::str(n, "source");
                                if (!s) say::fatal(1, "failed to get {} field {} as a string!", say::bolden(link.name), say::bolden("source"));
                                else link.source = fs::path(s.value());
                            } else if (ucl::check(n, "source") &&  n["source"].type() != ucl::String) {
                                say::fatal(1, "{} field {} must be a string!", say::bolden(link.name), say::bolden("source"));
                            } else {
                                say::fatal(1, "link {} is missing a {} value!", say::bolden(link.name), say::bolden("source"));
                            }
                            // END source

                            // BEGIN dest/destdir
                            if (ucl::check(n, "dest")) {
                                auto s = ucl::get::str(n, "dest");
                                if (!s) say::fatal(1, "failed to parse {} field {} as a string!", say::bolden(link.name), say::bolden("dest"));
                                else link.destination = fs::path(s.value());
                            } else if (ucl::check(n, "destdir")) {
                                auto s  = ucl::get::str(n, "destdir");
                                if (!s) say::fatal(1, "failed to parse {} field {} as a string!", say::bolden(link.name), say::bolden("destdir"));
                                std::string bn = link.source.filename().string();    
                                link.destination = fs::path(std::format("{}/{}", s.value(), bn));
                            } else {
                                say::fatal(1, "link {} is missing a {} value!", say::bolden(link.name), say::bolden("dest/destdir"));
                            }
                            // END dest/destdir
                            
                            // BEGIN type
                            if (!ucl::check(n, "type")) {
                                link.type = confidant::config::linkType::file;
                            } else {
                                auto t = ucl::get::str(n, "type");
                                if (!t) say::fatal(1, "failed to parse {} field {} as a string!", say::bolden(link.name), say::bolden("type"));
                                else if (t.value() == "file") link.type = confidant::config::linkType::file;
                                else if (t.value() == "directory") link.type = confidant::config::linkType::directory;
                                else {
                                    say::warn("type {} is not recognized, expected one of {} or {}, using default",
                                        say::ital(t.value()),
                                        say::bolden("file"),
                                        say::bolden("directory"));
                                    
                                    link.type = confidant::config::linkType::file;
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
                    say::warnextra("field {} not specified", say::bolden("templates"));
                } else {
                    auto oobj = ucl::get::node(input, "templates");
                    if (!oobj) {
                        say::fatal(1, "failed to parse {} as a node!", say::bolden("templates"));
                    } else {
                        ucl::Ucl obj = oobj.value();
                        int numtemplates = ucl::members(obj);
                        conf.templates.reserve(numtemplates);
                        
                        for (const auto& tmpl : obj) {
                            confidant::config::templatelink t;
                            
                            t.name = tmpl.key();
                            
                            if (ucl::check(tmpl, "source")) {
                                t.source = fs::path(tmpl["source"].string_value());
                            
                            } else {
                                say::fatal(1,
                                    "template {} is missing a {} value!",
                                    say::bolden(t.name),
                                    say::ital("source"));
                                
                            }
                                
                            if (ucl::check(tmpl, "dest")) {
                                t.destination = fs::path(tmpl["dest"].string_value());
                            
                            } else {
                                say::fatal(1,
                                    "template {} is missing a {} value!",
                                    say::bolden(t.name),
                                    say::ital("dest"));
                            }
                            
                            if (ucl::check(tmpl, "items") && tmpl["items"].type() == ucl::Array) {
                                // items field exists and is an array (normal, correct)
                                int numitems = tmpl["items"].size();
                                t.items.reserve(numitems);
                                
                                for (int i = 0; i < numitems; i++) {
                                    auto lst = ucl::get::list(tmpl, "items");
                                    if (!lst) {
                                        say::fatal(1,
                                            "failed to get {} items field as a list!",
                                            say::bolden(t.name));
                                    } else {
                                        t.items.push_back(lst.value().at(i).string_value());
                                    }
                                }
                                
                            } else if (ucl::check(tmpl, "items") && tmpl["items"].type() != ucl::Array) {
                                // items field exists, but is not an array (fatal)
                                say::fatal(1,
                                    "template {} {} field is not a list!",
                                    say::bolden(t.name),
                                    say::ital("items"));
                            } else {
                                // items field doesn't exist (fatal)
                                say::fatal(1,
                                    "template {} has no {} list!",
                                    say::bolden(t.name),
                                    say::ital("items"));
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
        
        namespace global {
            confidant::settings serialize(std::string_view path) {
                confidant::settings config;
                confidant::settings configdefs = confidant::config::global::defaults();
                
                auto object = ucl::parsing::file(path, std::map<std::string, std::string>{});
                
                if (!ucl::check(object, "create-directories")) {
                    config.createdirs = configdefs.createdirs;
                } else {
                    auto n = ucl::get::boolean(object, "create-directories");
                    if (!n) config.createdirs = configdefs.createdirs;
                    else config.createdirs = n.value();
                }
                
                if (!ucl::check(object, "color")) {
                    config.color = configdefs.color;
                } else {
                    auto n = ucl::get::boolean(object, "color");
                    if (!n) config.color = configdefs.color;
                    else if (!usecolor) config.color = false;
                    else config.color = n.value();
                }
                
                if (!ucl::check(object, "log-level")) {
                    config.loglevel = configdefs.loglevel;
                    
                } else if (object["log-level"].type() == ucl::String) {
                    auto n = ucl::get::str(object, "log-level");
                    if (!n) config.loglevel = configdefs.loglevel;
                    else {
                        std::string v = n.value();
                        if (v == "quiet") config.loglevel = verbose::quiet;
                        else if (v == "normal") config.loglevel = verbose::normal;
                        else if (v == "info") config.loglevel = verbose::info;
                        else if (v == "debug") config.loglevel = verbose::debug;
                        else if (v == "trace") config.loglevel = verbose::trace;
                        else config.loglevel = configdefs.loglevel;
                    }
                    
                } else if (object["log-level"].type() == ucl::Int) {
                    auto n = ucl::get::integer(object, "log-level");
                    if (!n) config.loglevel = configdefs.loglevel;
                    else {
                        switch (n.value()) {
                            case 0:
                                config.loglevel = verbose::quiet;
                                break;
                            case 1:
                                config.loglevel = verbose::normal;
                                break;
                            case 2:
                                config.loglevel = verbose::info;
                                break;
                            case 3:
                                config.loglevel = verbose::debug;
                                break;
                            case 4:
                                config.loglevel = verbose::trace;
                                break;
                            default:
                                config.loglevel = configdefs.loglevel;
                                break;
                        }
                    }
                    
                } else {
                    config.loglevel = configdefs.loglevel;
                }
                
                return config;
            }
        }; // END confidant::config::global
    }; // END confidant::config
};