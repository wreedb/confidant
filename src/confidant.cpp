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
            
            if (!fs::exists(path(sourcestr))) {
                logger::error(PROJECT_NAME, "source file {} does not exist!", usourcestr);
                continue;
            }
            
            if (fs::exists(deststr)) {
                if (fs::equivalent(sourcestr, deststr)) {
                    logger::warn(PROJECT_NAME, "skipping {}, already linked", udeststr);
                    continue;
                }
                logger::warn(PROJECT_NAME, "destination {} already exists, skipping", udeststr);
                continue;
            }
            
            if (!fs::exists(destpath.parent_path())) {
                if (!conf.create_dirs) {
                    logger::warn(PROJECT_NAME, "parent directory for {} doesn't exist, skipping", udeststr);
                    continue;
                } else {
                    if (!dry) {
                        try {
                            fs::create_directories(destpath.parent_path());
                        } catch (const fs::filesystem_error& err) {
                            logger::error(PROJECT_NAME, "failed to create directory {}", util::unexpandhome(destpath.parent_path().string()));
                            cout << err.what() << "\n";
                            std::exit(1);
                        }
                    }
                    logger::extra(PROJECT_NAME, "created directory {}", util::unexpandhome(destpath.parent_path().string()));
                }
            }
            
            if (!util::perms_to_link(destpath) && !dry) {
                logger::error(PROJECT_NAME, "no write permission for directory '{}'", util::unexpandhome(destpath.parent_path().string()));
                continue;
            }
            
            if (fs::status(sourcepath).type() == fs::file_type::directory) {
                if (!dry) {
                    try {
                        fs::create_directory_symlink(sourcepath, destpath);
                    } catch (const fs::filesystem_error& err) {
                        logger::error(PROJECT_NAME, "failed to create symlink at {}", udeststr);
                        std::cout << err.what() << std::endl;
                    }
                }
                processedItems++;
            } else {
                if (!dry) {
                    try {
                        fs::create_symlink(sourcepath, destpath);
                    } catch (const fs::filesystem_error& err) {
                        logger::error(PROJECT_NAME, "failed to create symlink at {}", udeststr);
                        std::cout << err.what() << std::endl;
                    }
                }
                processedItems++;
            }
            logger::info(PROJECT_NAME, "linked {}", udeststr);
            
        }
        logger::extra(PROJECT_NAME, "processed {} items", processedItems);
        processedTemplates++;
    }
    
    logger::extra(PROJECT_NAME, "processed {} temlates", processedTemplates);
    
    return 0;
}
    
// int linkfrom(const confidant::configuration& conf, const bool& dry) {
//     int numitems = conf.linkFrom.items.size();
//     int itemslinked = 0;
//     for (int n = 0; n < numitems; n++) {
//         std::string from = confidant::substitute(conf.linkFrom.from, conf.linkFrom.items.at(n));
//         std::string to   = confidant::substitute(conf.linkFrom.to, conf.linkFrom.items.at(n));
//         std::string ufrom = fs::relative(path(from)).string();
//         std::string uto   = util::unexpandhome(to);
//         if (!fs::exists(path(from))) {
//             logger::error(PROJECT_NAME, "source file '{}' does not exist!", ufrom);
//             continue;
//         }
//         if (fs::exists(to)) {
//             if (fs::equivalent(from, to)) {
//                 logger::warn(PROJECT_NAME, "skipping '{}', already linked", uto);
//                 continue;
//             }
//             logger::warn(PROJECT_NAME, "destination '{}', already exists, skipping", uto);
//             continue;
//         }
        
//         if (!fs::exists(fs::path(to).parent_path())) {
//             if (!conf.create_dirs) {
//                 logger::warn(PROJECT_NAME, "parent directory for {} does not exist, skipping", uto);
//                 continue;
//             } else {
//                 if (!dry) {
//                     try {
//                         fs::create_directories(fs::path(to).parent_path());
//                     } catch (const fs::filesystem_error& err) {
//                         logger::error(PROJECT_NAME, "no write permission for directory '{}'", util::unexpandhome(path(to).parent_path().string()));
//                         cout << err.what() << "\n";
//                         std::exit(1);
//                     }
//                 }
//                 logger::extra(PROJECT_NAME, "created directory {}", util::unexpandhome(path(to).parent_path().string()));
//             }
//         }
        
//         if (!util::perms_to_link(path(to)) && !dry) {
//             logger::error(PROJECT_NAME, "no write permission for directory '{}'", util::unexpandhome(path(to).parent_path().string()));
//             continue;
//         }
        
//         if (fs::status(path(from)).type() == fs::file_type::directory) {
//             if (!dry) {
//                 try {
//                     fs::create_directory_symlink(path(from), path(to));
//                 } catch (const fs::filesystem_error& err) {
//                     logger::error(PROJECT_NAME, "failed to create symlink at '{}'", uto);
//                     std::cout << err.what() << std::endl;
//                 }
//             }
//             itemslinked++;
//         } else {
//             if (!dry) {
//                 try {
//                     fs::create_symlink(path(from), path(to));
//                 } catch (const fs::filesystem_error& err) {
//                     logger::error(PROJECT_NAME, "failed to create symlink at '{}'", uto);
//                     std::cout << err.what() << std::endl;
//                 }
//             }
//             itemslinked++;
//         }
    
//         logger::info(PROJECT_NAME, "link-from {} -> {}", ufrom, uto);
//     }
    
//     logger::extra(PROJECT_NAME, "linked {} items", itemslinked);
//     return 0;
// }
    
int link(const confidant::configuration& conf, const bool& dry) {
    using util::unexpandhome;
    int numlinks = conf.links.size();
    int linksdone = 0;
    
    for (int n = 0; n < numlinks; n++) {
        
        std::string src = conf.links.at(n).source.string();
        std::string dest = conf.links.at(n).destination.string();
        std::string usrc = fs::relative(path(src)).string();
        std::string udest = util::unexpandhome(dest);
        
        confidant::config::linkType linkType = conf.links.at(n).type;
        
        fs::file_status src_stat = fs::status(conf.links.at(n).source);
        
        // skip if the source file doesn't exist
        if (!fs::exists(path(conf.links.at(n).source))) {
            logger::error(PROJECT_NAME, "source file '{}' does not exist!", usrc);
            continue;
        }
        
        // skip if the output file already exists
        
        if (fs::exists(path(conf.links.at(n).destination))) {
            // if the source and dest are the same file, e.g. the link was already made before
            if (fs::equivalent(conf.links.at(n).source, conf.links.at(n).destination)) {
                logger::warn(PROJECT_NAME, "skipping '{}', already linked", udest);
                continue;
            }
            
            // the files are not the same, just warn and skip
            logger::warn(PROJECT_NAME, "destination '{}', already exists, skipping", udest);
            continue;
        }
        
        if (!fs::exists(fs::path(dest).parent_path())) {
            if (!conf.create_dirs) {
                logger::warn(PROJECT_NAME, "parent directory for {} does not exist, skipping", conf.links.at(n).name);
                continue;
            } else {
                if (!dry) {
                    try {
                        fs::create_directories(fs::path(dest).parent_path());
                    } catch (const fs::filesystem_error& err) {
                        logger::error(PROJECT_NAME, "could not create directory {}", unexpandhome(path(dest).parent_path().string()));
                        cout << err.what() << "\n";
                        std::exit(1);
                    }
                }
                logger::extra(PROJECT_NAME, "created directory {}", unexpandhome(path(dest).parent_path().string()));
            }
        }
        
        // check parent directory for write permission
        if (!util::perms_to_link(fs::path(dest)) && !dry) {
            logger::error(PROJECT_NAME, "no write permissions for {}", unexpandhome(path(dest).parent_path().string()));
            continue;
        }
        
        // use create_directory_symlink for dirs
        if (linkType == confidant::config::linkType::directory) {
            
            if (src_stat.type() != fs::file_type::directory) {
                logger::error(PROJECT_NAME, "{} source '{}' is not a directory", conf.links.at(n).name, usrc);
                continue;
            }
            if (!dry) {
                try {
                    fs::create_directory_symlink(src, dest);
                } catch (const fs::filesystem_error& err) {
                    logger::error(PROJECT_NAME, "failed to create symlink for {} at {}", conf.links.at(n).name, udest);
                    std::cout << err.what() << std::endl;
                }
            }
            linksdone++;
            
        } else if (linkType == confidant::config::linkType::file) {
            if (!dry) {
                try {
                    fs::create_symlink(src, dest);
                } catch (const fs::filesystem_error& err) {
                    logger::error(PROJECT_NAME, "failed to create symlink for {} at {}", conf.links.at(n).name, udest);
                    std::cout << err.what() << std::endl;
                }
            }
            linksdone++;
        } else {
            std::unreachable();
        }
        
        // the file was linked
        logger::info(PROJECT_NAME, "link {} -> {}", usrc, udest);    
    }
    
    logger::extra(PROJECT_NAME, "created {} links", linksdone);
    
    return 0;

}
    
namespace debug {
namespace global {
void dumpConfig(const confidant::settings& conf) {
    logger::info(PROJECT_NAME, "global configuration");
    std::println("  create-directories: {}", conf.create_dirs);
    std::println("  log-level: {}", ansi::verbosity_literal(conf.loglevel));
    std::cout << std::endl;
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
                    std::unreachable();
            }
        }
    }
    if (numtemplates > 0) {
        std::println("  templates:");
        for (int n = 0; n < numlinks; n++) {
            std::println("    {}:", conf.templates.at(n).name);
            std::println("      source: {}", std::string(conf.templates.at(n).source));
            std::println("      destination: {}", std::string(conf.templates.at(n).destination));
            int numitems = conf.templates.at(n).items.size();
            for (int x = 0; x < numitems; x++) {
                std::println("      - {}", conf.templates.at(n).items.at(x));
            }
        }
    }
}

} // NOTE: end namespace confidant::debug

} // NOTE: end namespace confidant