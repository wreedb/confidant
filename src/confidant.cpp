// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <utility>
#include <vector>
#include <string>
#include <format>
#include <iostream>
#include <print>

#include "settings.hpp"
#include "util.hpp"
#include "ucl.hpp"
#include "logging.hpp"

using std::string;
using std::filesystem::path;
namespace fs = std::filesystem;

namespace say = confidant::logging;

namespace confidant {

    int linktemplate(const confidant::configuration& conf, const bool& dry) {
        int processedTemplates = 0;
        for (auto& tmpl : conf.templates) {
      
            int processedItems = 0;
            int numitems = tmpl.items.size();
            
            for (int n = 0; n < numitems; n++) {
                
                std::string sourcestr = confidant::substitute(tmpl.source.string() , tmpl.items.at(n));
                std::string deststr   = confidant::substitute(tmpl.destination.string(), tmpl.items.at(n));
                
                fs::path sourcepath = path(sourcestr);
                fs::path destpath = path(deststr);
                
                std::string usourcestr = fs::relative(path(sourcestr)).string();
                std::string udeststr = util::unexpandhome(deststr);
                
                if (!fs::exists(sourcepath)) {
                    say::error("source file {} does not exist!",
                        say::bolden(udeststr));
                    continue;
                }
                
                if (fs::exists(destpath)) {
                    if (fs::equivalent(sourcepath, destpath)) {
                        say::warn("skipping {}, already linked",
                            say::bolden(udeststr));
                        continue;
                    }
                    say::warn("destination {} already exists, skipping",
                        say::bolden(udeststr));
                    continue;
                }
                
                if (!fs::exists(destpath.parent_path())) {
                    if (!conf.create_dirs) {
                        say::warn("parent directory for {} doesn't exist, skipping", say::bolden(udeststr));
                        continue;
                    } else {
                        if (!dry) {
                            try {
                                fs::create_directories(destpath.parent_path());
                            } catch (const fs::filesystem_error& err) {
                                say::error("failed to create directory {}",
                                    say::bolden(util::unexpandhome(destpath.parent_path().string())));
                                cout << err.what() << std::endl;
                                return 1;
                            }
                        }
                        say::extra("created directory {}",
                            say::bolden(util::unexpandhome(destpath.parent_path().string())));
                    }
                }
                
                if (!util::hasperms(deststr) && !dry) {
                    say::error("no write permissions for directory {}",
                        say::bolden(util::unexpandhome(destpath.parent_path().string())));
                    continue;
                }
                
                if (fs::status(sourcepath).type() == fs::file_type::directory) {
                    if (!dry) {
                        try {
                            // create the link
                            fs::create_directory_symlink(sourcepath, destpath);
                        } catch (const fs::filesystem_error& err) {
                            say::error("failed to create symlink at {}", say::bolden(udeststr));
                            cout << err.what() << std::endl;
                            return 1;
                        }
                    }
                    processedItems++;
                } else {
                    if (!dry) {
                        try {
                            // create the link
                            fs::create_symlink(sourcepath, destpath);
                        } catch (const fs::filesystem_error& err) {
                            say::error("failed to create symlink at {}", say::bolden(udeststr));
                            std::cout << err.what() << std::endl;
                            return 1;
                        }
                    }
                    processedItems++;
                }
                // show message regardless for dry-runs
                std::println("{} linked {}",
                    say::fg::magenta(">>>"),
                    say::bolden(udeststr)
                );
            }
            say::extra("processed {} items", processedItems);
            processedTemplates++;
        }
        say::extra("processed {} templates", processedTemplates);
        
        return 0;
    }
    
    int link(const confidant::configuration& conf, const bool& dry) {
        using util::unexpandhome;
        int numlinks = conf.links.size();
        int linksdone = 0;
        
        for (int n = 0; n < numlinks; n++) {
            
            std::string name       = conf.links.at(n).name;
            fs::path sourcepath    = conf.links.at(n).source;
            fs::path destpath      = conf.links.at(n).destination;
            std::string sourcestr  = sourcepath.string();
            std::string deststr    = destpath.string();
            std::string usourcestr = fs::relative(sourcepath).string();
            std::string udeststr   = util::unexpandhome(deststr);
            
            confidant::config::linkType linkType = conf.links.at(n).type;
            
            // skip if the source file doesn't exist
            if (!fs::exists(sourcepath)) {
                std::println(std::cerr,
                    "{}: source file {} does not exist!",
                    say::fg::red("error"),
                    say::bolden(usourcestr)
                );
                continue;
            }
            // only after checking the file exists
            fs::file_status sourcefstat = fs::status(sourcepath);
            
            // check if the destination exists
            if (fs::exists(destpath)) {
                // if the source and dest are the same file, e.g. the link was (likely) already created by us
                if (fs::equivalent(sourcepath, destpath)) {
                    say::warn("skipping {}, already linked",
                        say::bolden(udeststr));
                    continue;
                }
                
                // the destination already exists, and isn't identical to our source; we do nothing and skip
                say::warn("destination {} already exists and is not identical to source, skipping",
                    say::bolden(udeststr));
                continue;
            }
            
            if (!fs::exists(destpath.parent_path())) {
                if (!conf.create_dirs) {
                    // parent path doesn't exist and settings to create dirs is off
                    say::warn("parent directory for {} does not exist, skipping",
                        say::bolden(name));
                    continue;
                } else {
                    // create dirs unless we are doing a dry run
                    if (!dry) {
                        try {
                            fs::create_directories(destpath.parent_path());
                        } catch (const fs::filesystem_error& err) {
                            say::error("failed to create directory {}",
                                say::bolden(unexpandhome(destpath.parent_path().string())));
                            cout << err.what() << "\n";
                            return 1;
                        }
                    }
                    // display extra message regardless, for dry-run verbose
                    say::extra("create directory {}",
                        say::bolden(unexpandhome(destpath.parent_path().string())));
                }
            }
            
            // check parent directory for write permission
            if (!util::hasperms(deststr) && !dry) {
                say::error("no write permissions for {}",
                    say::bolden(unexpandhome(destpath.parent_path().string())));
                // TODO: add strict setting, continue if true, return 1 if false
                continue;
            }
            
            // use create_directory_symlink for dirs because apparenty
            // some inferior operating systems treat directory symlinks
            // differently to file symlinks
            if (linkType == confidant::config::linkType::directory) {
                
                if (sourcefstat.type() != fs::file_type::directory) {
                    // they specified directory, but the source is not a directory
                    say::error("link {} source {} is not a directory",
                        say::bolden(name),
                        say::ital(usourcestr));
                    // TODO: strict return 1
                    continue;
                }
                if (!dry) {
                    try {
                        // create the link
                        fs::create_directory_symlink(sourcepath, destpath);
                    } catch (const fs::filesystem_error& err) {
                        say::error("failed to create symlink for {} at {}",
                            say::bolden(name),
                            say::ital(udeststr));
                        std::cout << err.what() << std::endl;
                        // TODO: continue when strict == false
                        return 1;
                    }
                }
                linksdone++;
                
            } else if (linkType == confidant::config::linkType::file) {
                if (!dry) {
                    try {
                        // create the link
                        fs::create_symlink(sourcepath, destpath);
                    } catch (const fs::filesystem_error& err) {
                        say::error("failed to create symlink for {} at {}",
                            say::bolden(name),
                            say::ital(udeststr));
                        cout << err.what() << std::endl;
                        // TODO: continue if strict == false
                        return 1;
                    }
                }
                // incr the tracking counter
                linksdone++;
            } else {
                // cant happen, the linkType will have already been resolved to a good value 
                std::unreachable();
            }
            
            // the file was linked
            say::pretty("linked {}", udeststr);
        }
    
        say::extra("created {} normal links", say::bolden(std::to_string(linksdone)));
        return 0;

    }
    
    namespace debug {

        namespace global {

            void dumpConfig(const confidant::settings& conf) {
                say::pretty("global configuration");
                std::println("{}: {}", say::fg::blue("create-directories"), conf.createdirs);
                std::println("{}: {}", say::fg::blue("log-level"), ansi::verbosity_literal(int(conf.loglevel)));
                std::println("{}: {}", say::fg::blue("color"), conf.color);
            }

        }; // END confidant::debug::global
        
        void dumpConfig(const confidant::configuration& conf) {
            std::println("{}:", say::fg::blue("repository"));
            std::println("  {}: {}", say::fg::blue("url"), conf.repo.url);
            int numlinks = conf.links.size();
            int numtemplates = conf.templates.size();
            if (numlinks > 0) {
                std::println("{}:", say::fg::blue("links"));
                for (int n = 0; n < numlinks; n++) {
                    std::println("- {}: {}", say::fg::blue("name"), conf.links.at(n).name);
                    std::println("  {}: {}", say::fg::blue("source"), conf.links.at(n).name);
                    std::println("  {}: {}", say::fg::blue("destination"), conf.links.at(n).destination.string());
                    switch (conf.links.at(n).type) {
                        case confidant::config::linkType::file:
                            std::println("  {}: file", say::fg::blue("type"));
                            break;
                        case confidant::config::linkType::directory:
                            std::println("  {}: directory", say::fg::blue("type"));
                            break;
                        default:
                            // invalid or absent values will have been replaced with 'file'
                            std::unreachable();
                    }
                }
            }
            
            if (numtemplates > 0) {
                std::println("{}:", say::fg::blue("templates"));
                for (int n = 0; n < numtemplates; n++) {
                    std::string name = conf.templates.at(n).name;
                    std::string src = conf.templates.at(n).source.string();
                    std::string dst = conf.templates.at(n).destination.string();
                    std::println("- {}: {}", say::fg::blue("name"), name);
                    std::println("  {}: {}", say::fg::blue("source"), src);
                    std::println("  {}: {}", say::fg::blue("destination"), dst);
                    int numitems = conf.templates.at(n).items.size();
                    if (numitems > 0) {
                        std::println("  {}:", say::fg::blue("items"));
                        for (int x = 0; x < numitems; x++) {
                            std::println("  - {}", say::fg::green(conf.templates.at(n).items.at(x)));
                        }
                    }
                }
            }
        }

    } // END confidant::debug

} // END confidant