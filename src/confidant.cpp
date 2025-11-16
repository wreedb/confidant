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

#include "confidant.hpp"
#include "util.hpp"
#include "ucl.hpp"
#include "logging.hpp"
#include "config.hpp"

using std::string;
using std::filesystem::path;
namespace fs = std::filesystem;

namespace logger = confidant::logging;

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
                    logger::error(PROJECT_NAME,
                        "source file {}{}{} does not exist!",
                        logger::color(1),
                        usourcestr,
                        ansi::freset);
                    continue;
                }
                
                if (fs::exists(destpath)) {
                    if (fs::equivalent(sourcepath, destpath)) {
                        logger::warn(PROJECT_NAME,
                            "skipping {}{}{}, already linked",
                            logger::color(1),
                            udeststr,
                            ansi::freset);
                        continue;
                    }
                    logger::warn(PROJECT_NAME,
                        "destination {}{}{} already exists, skipping",
                        logger::color(1),
                        udeststr,
                        ansi::freset);
                    continue;
                }
                
                if (!fs::exists(destpath.parent_path())) {
                    if (!conf.create_dirs) {
                        logger::warn(PROJECT_NAME,
                            "parent directory for {}{}{} doesn't exist, skipping",
                            logger::color(1),
                            udeststr,
                            ansi::freset);
                        continue;
                    } else {
                        if (!dry) {
                            try {
                                fs::create_directories(destpath.parent_path());
                            } catch (const fs::filesystem_error& err) {
                                logger::error(PROJECT_NAME,
                                    "failed to create directory {}{}{}",
                                    logger::color(1),
                                    util::unexpandhome(destpath.parent_path().string()),
                                    ansi::freset);
                                cout << err.what() << std::endl;
                                return 1;
                            }
                        }
                        logger::extra(PROJECT_NAME,
                            "created directory {}{}{}",
                            logger::color(1),
                            util::unexpandhome(destpath.parent_path().string()),
                            ansi::freset);
                    }
                }
                
                if (!util::perms_to_link(destpath) && !dry) {
                    logger::error(PROJECT_NAME,
                        "no write permission for directory {}{}{}",
                        logger::color(1),
                        util::unexpandhome(destpath.parent_path().string()),
                        ansi::freset);
                    continue;
                }
                
                if (fs::status(sourcepath).type() == fs::file_type::directory) {
                    if (!dry) {
                        try {
                            // create the link
                            fs::create_directory_symlink(sourcepath, destpath);
                        } catch (const fs::filesystem_error& err) {
                            logger::error(PROJECT_NAME,
                                "failed to create symlink at {}{}{}",
                                logger::color(1),
                                udeststr,
                                ansi::freset);
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
                            logger::error(PROJECT_NAME,
                                "failed to create symlink at {}{}{}",
                                logger::color(1),
                                udeststr,
                                ansi::freset);
                            std::cout << err.what() << std::endl;
                            return 1;
                        }
                    }
                    processedItems++;
                }
                // show message regardless for dry-runs
                logger::info(PROJECT_NAME,
                    "linked {}{}{}",
                    logger::color(1),
                    udeststr,
                    ansi::freset);
            }
            logger::extra(PROJECT_NAME, "processed {} items", processedItems);
            processedTemplates++;
        }
        
        logger::extra(PROJECT_NAME, "processed {} templates", processedTemplates);
        
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
                logger::error(PROJECT_NAME,
                    "source file {}{}{} does not exist!",
                    logger::color(1),
                    usourcestr,
                    ansi::freset);
                continue;
            }
            // only after checking the file exists
            fs::file_status sourcefstat = fs::status(sourcepath);
            
            // check if the destination exists
            if (fs::exists(destpath)) {
                // if the source and dest are the same file, e.g. the link was (likely) already created by us
                if (fs::equivalent(sourcepath, destpath)) {
                    logger::warn(PROJECT_NAME,
                        "skipping {}{}{}, already linked",
                        logger::color(1),
                        udeststr,
                        ansi::freset);
                    continue;
                }
                
                // the destination already exists, and isn't identical to our source; we do nothing and skip
                logger::warn(PROJECT_NAME,
                    "destination {}{}{}, already exists, skipping",
                    logger::color(1),
                    udeststr,
                    ansi::freset);
                continue;
            }
            
            if (!fs::exists(destpath.parent_path())) {
                if (!conf.create_dirs) {
                    // parent path doesn't exist and settings to create dirs is off
                    logger::warn(PROJECT_NAME,
                        "parent directory for {}{}{} does not exist, skipping",
                        logger::color(1),
                        name,
                        ansi::freset);
                    continue;
                } else {
                    // create dirs unless we are doing a dry run
                    if (!dry) {
                        try {
                            fs::create_directories(destpath.parent_path());
                        } catch (const fs::filesystem_error& err) {
                            logger::error(PROJECT_NAME,
                                "failed to create directory {}{}{}",
                                logger::color(1),
                                unexpandhome(destpath.parent_path().string()),
                                ansi::freset);
                            cout << err.what() << "\n";
                            return 1;
                        }
                    }
                    // display extra message regardless, for dry-run verbose
                    logger::extra(PROJECT_NAME,
                        "created directory {}{}{}",
                        logger::color(1),
                        unexpandhome(destpath.parent_path().string()),
                        ansi::freset);
                }
            }
            
            // check parent directory for write permission
            if (!util::perms_to_link(destpath) && !dry) {
                logger::error(PROJECT_NAME,
                    "no write permissions for {}{}{}",
                    logger::color(1),
                    unexpandhome(destpath.parent_path().string()),
                    ansi::freset);
                continue;
            }
            
            // use create_directory_symlink for dirs because apparenty
            // some inferior operating systems treat directory symlinks
            // differently to file symlinks
            if (linkType == confidant::config::linkType::directory) {
                
                if (sourcefstat.type() != fs::file_type::directory) {
                    // they specified directory, but the source is not a directory
                    logger::error(PROJECT_NAME,
                        "link {}{}{} source {}{}{} is not a directory",
                        logger::color(1),
                        name,
                        ansi::freset,
                        logger::color(3),
                        usourcestr,
                        ansi::freset);
                    continue;
                }
                if (!dry) {
                    try {
                        // create the link
                        fs::create_directory_symlink(sourcepath, destpath);
                    } catch (const fs::filesystem_error& err) {
                        logger::error(PROJECT_NAME,
                            "failed to create symlink for {}{}{} at {}{}{}",
                            logger::color(1),
                            name,
                            ansi::freset,
                            logger::color(3),
                            udeststr,
                            ansi::freset);
                        std::cout << err.what() << std::endl;
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
                        logger::error(PROJECT_NAME,
                            "failed to create symlink for {}{}{} at {}{}{}",
                            logger::color(1),
                            name,
                            ansi::freset,
                            logger::color(3),
                            udeststr,
                            ansi::freset);
                        cout << err.what() << std::endl;
                        return 1;
                    }
                }
                // incr the tracking counter
                linksdone++;
            } else {
                // cant happen, the linkType will have already been set to 
                // file by default if it was invalid.
                std::unreachable();
            }
            
            // the file was linked
            logger::info(PROJECT_NAME, "linked {}{}{}",
                logger::color(1),
                udeststr,
                ansi::freset);
        }
    
        logger::extra(PROJECT_NAME, "created {} normal links", linksdone);
        return 0;

    }
    
    namespace debug {

        namespace global {

            void dumpConfig(const confidant::settings& conf) {
                logger::info(PROJECT_NAME, "global configuration");
                std::println("  create-directories: {}", conf.create_dirs);
                std::println("  log-level: {}", ansi::verbosity_literal(conf.loglevel));
            }

        }; // END confidant::debug::global
        
        void dumpConfig(const confidant::configuration& conf) {
            std::println("  repository: ");
            std::println("    url: {}", conf.repo.url);
            int numlinks = conf.links.size();
            int numtemplates = conf.templates.size();
            if (numlinks > 0) {
                std::println("  link:");
                for (int n = 0; n < numlinks; n++) {
                    std::println("  - name: {}", conf.links.at(n).name);
                    std::println("    source: {}", conf.links.at(n).source.string());
                    std::println("    destination: {}", conf.links.at(n).destination.string());
                    switch (conf.links.at(n).type) {
                        case confidant::config::linkType::file:
                            std::println("    type: file");
                            break;
                        case confidant::config::linkType::directory:
                            std::println("    type: directory");
                            break;
                        default:
                            // invalid or absent values will have been replaced with 'file'
                            std::unreachable();
                    }
                }
            }
            
            if (numtemplates > 0) {
                std::println("  templates:");
                for (int n = 0; n < numtemplates; n++) {
                    std::println("    {}:", conf.templates.at(n).name);
                    std::println("      source: {}", conf.templates.at(n).source.string());
                    std::println("      destination: {}", conf.templates.at(n).destination.string());
                    int numitems = conf.templates.at(n).items.size();
                    for (int x = 0; x < numitems; x++) {
                        std::println("      - {}", conf.templates.at(n).items.at(x));
                    }
                }
            }
        }

    } // END confidant::debug

} // END confidant