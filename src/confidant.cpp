// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
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

int linkfrom(const confidant::configuration& conf, const bool& dry) {
    int numitems = conf.linkFrom.items.size();
    int itemslinked = 0;
    for (int n = 0; n < numitems; n++) {
        std::string from = confidant::substitute(conf.linkFrom.from, conf.linkFrom.items.at(n));
        std::string to   = confidant::substitute(conf.linkFrom.to, conf.linkFrom.items.at(n));
        
        if (!fs::exists(path(from))) {
            logger::error(PROJECT_NAME, "source file '{}' does not exist!", from);
            continue;
        }
        
        if (fs::exists(to)) {
            if (fs::equivalent(from, to)) {
                logger::warn(PROJECT_NAME, "skipping '{}', already linked", to);
                continue;
            }
            logger::warn(PROJECT_NAME, "destination '{}', already exists, skipping", to);
            continue;
        }
        
        if (!fs::exists(fs::path(to).parent_path())) {
            if (!conf.create_dirs) {
                logger::warn(PROJECT_NAME, "parent directory for {} does not exist, skipping", conf.linkFrom.to.parent_path().string());
                continue;
            } else {
                if (!dry) {
                    try {
                        fs::create_directories(fs::path(to).parent_path());
                    } catch (const fs::filesystem_error& err) {
                        logger::error(PROJECT_NAME, "no write permission for directory '{}'", path(to).parent_path().string());
                        cout << err.what() << "\n";
                        std::exit(1);
                    }
                }
                logger::info(PROJECT_NAME, "created directory {}", path(to).parent_path().string());
            }
        }
        
        if (!util::perms_to_link(path(to)) && !dry) {
            logger::error(PROJECT_NAME, "no write permission for directory '{}'", path(to).parent_path().string());
            continue;
        }
        
        if (fs::status(path(from)).type() == fs::file_type::directory) {
            if (!dry) {
                try {
                    fs::create_directory_symlink(path(from), path(to));
                } catch (const fs::filesystem_error& err) {
                    logger::error(PROJECT_NAME, "failed to create symlink at '{}'", to);
                    std::cout << err.what() << std::endl;
                }
            }
            itemslinked++;
        } else {
            if (!dry) {
                try {
                    fs::create_symlink(path(from), path(to));
                } catch (const fs::filesystem_error& err) {
                    logger::error(PROJECT_NAME, "failed to create symlink at '{}'", to);
                    std::cout << err.what() << std::endl;
                }
            }
            itemslinked++;
        }
    
        logger::info(PROJECT_NAME, "link-from {} -> {}", from, to);
    }
    
    logger::extra(PROJECT_NAME, "linked {} items", itemslinked);
    return 0;
}
    
int link(const confidant::configuration& conf, const bool& dry) {
    int numlinks = conf.links.size();
    int linksdone = 0;
    
    for (int n = 0; n < numlinks; n++) {
        
        std::string src = conf.links.at(n).source.string();
        std::string dest = conf.links.at(n).destination.string();
        confidant::config::linkType linkType = conf.links.at(n).type;
        
        fs::file_status src_stat = fs::status(conf.links.at(n).source);
        
        // skip if the source file doesn't exist
        if (!fs::exists(path(conf.links.at(n).source))) {
            logger::error(PROJECT_NAME, "source file '{}' does not exist!", src);
            continue;
        }
        
        // skip if the output file already exists
        
        if (fs::exists(path(conf.links.at(n).destination))) {
            // if the source and dest are the same file, e.g. the link was already made before
            if (fs::equivalent(conf.links.at(n).source, conf.links.at(n).destination)) {
                logger::warn(PROJECT_NAME, "skipping '{}', already linked", dest);
                continue;
            }
            
            // the files are not the same, just warn and skip
            logger::warn(PROJECT_NAME, "destination '{}', already exists, skipping", dest);
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
                        logger::error(PROJECT_NAME, "could not create directory {}", path(dest).parent_path().string());
                        cout << err.what() << "\n";
                        std::exit(1);
                    }
                    logger::info(PROJECT_NAME, "created directory {}", path(dest).parent_path().string());
                }
            }
        }
        
        // check parent directory for write permission
        if (!util::perms_to_link(fs::path(dest)) && !dry) {
            logger::error(PROJECT_NAME, "no write permissions for {}", path(dest).parent_path().string());
            continue;
        }
        
        // use create_directory_symlink for dirs
        if (linkType == confidant::config::linkType::directory) {
            
            if (src_stat.type() != fs::file_type::directory) {
                logger::error(PROJECT_NAME, "{} source '{}' is not a directory", conf.links.at(n).name, src);
                continue;
            }
            if (!dry) {
                try {
                    fs::create_directory_symlink(src, dest);
                } catch (const fs::filesystem_error& err) {
                    logger::error(PROJECT_NAME, "failed to create symlink for {} at {}", conf.links.at(n).name, dest);
                    std::cout << err.what() << std::endl;
                }
            }
            linksdone++;
            
        } else if (linkType == confidant::config::linkType::file) {
            if (!dry) {
                try {
                    fs::create_symlink(src, dest);
                } catch (const fs::filesystem_error& err) {
                    logger::error(PROJECT_NAME, "failed to create symlink for {} at {}", conf.links.at(n).name, dest);
                    std::cout << err.what() << std::endl;
                }
            }
            linksdone++;
        } else {
            // this should never happen, but hey; what can ya do.
            logger::error(PROJECT_NAME, "error: link type for '{}' is not recognized, expected one of: file, directory", conf.links.at(n).name);
            continue;
        }
        
        // the file was linked
        logger::info(PROJECT_NAME, "link {} -> {}", src, dest);    
    }
    
    logger::extra(PROJECT_NAME, "created {} links", linksdone);
    
    return 0;

}
    
namespace debug {
void dumpConfig(const confidant::configuration& conf) {
    std::println("  repository: ");
    std::println("    url: {}", conf.repo.url);
    int numlinks = conf.links.size();
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
                    std::println("    type: unknown");
                    break;
            }
        }
    }
    
    int numitems = conf.linkFrom.items.size();
    if (numitems > 0) {
        std::println("  link-from:");
        std::println("    from: {}", std::string(conf.linkFrom.from));
        std::println("    to: {}", std::string(conf.linkFrom.to));
        std::println("    items:");
        for (int n = 0; n < numitems; n++) {
            std::println("    - {}", conf.linkFrom.items.at(n));
        }
    }
    
}

} // NOTE: end namespace confidant::debug

} // NOTE: end namespace confidant