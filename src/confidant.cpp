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

using std::string;
using std::filesystem::path;
namespace fs = std::filesystem;

namespace confidant {

int linkfrom(const confidant::configuration& conf) {
    int numitems = conf.linkFrom.items.size();
    for (int n = 0; n < numitems; n++) {
        std::string from = confidant::substitute(conf.linkFrom.from, conf.linkFrom.items.at(n));
        std::string to   = confidant::substitute(conf.linkFrom.to, conf.linkFrom.items.at(n));
        
        if (!fs::exists(path(from))) {
            std::println("error: source file '{}' does not exist!", from);
            continue;
        }
        
        if (fs::exists(to)) {
            if (fs::equivalent(from, to)) {
                std::println("confidant link-from: skipping '{}'", to);
                continue;
            }
            std::println("warning: destination '{}' exists and is not identical to source '{}'", to, from);
            continue;
        }
        
        if (!util::perms_to_link(path(to))) {
            std::println("error: no permission for directory '{}'", path(to).parent_path().string());
            continue;
        }
        
        if (fs::status(path(from)).type() == fs::file_type::directory) {
            try {
                fs::create_directory_symlink(path(from), path(to));
            } catch (const fs::filesystem_error& err) {
                std::println(std::cerr, "error: failed to create symlink at '{}'", to);
                std::cout << err.what() << std::endl;
            }
        } else {
            try {
                fs::create_symlink(path(from), path(to));
            } catch (const fs::filesystem_error& err) {
                std::println(std::cerr, "error: failed to create symlink at '{}'", to);
                std::cout << err.what() << std::endl;
            }
        }
        
        std::println("confidant link-from: {} -> {}", from, to);
    }
    
    return 0;
}
    
int link(const confidant::configuration& conf) {
    int numlinks = conf.links.size();
    
    for (int n = 0; n < numlinks; n++) {
        
        std::string src = conf.links.at(n).source.string();
        std::string dest = conf.links.at(n).destination.string();
        confidant::config::linkType linkType = conf.links.at(n).type;
        
        fs::file_status src_stat = fs::status(conf.links.at(n).source);
        
        // skip if the source file doesn't exist
        if (!fs::exists(path(conf.links.at(n).source))) {
            std::println(std::cerr, "warning: source file '{}' does not exist!", src);
            continue;
        }
        
        // skip if the output file already exists
        
        if (fs::exists(path(conf.links.at(n).destination))) {
            // if the source and dest are the same file, e.g. the link was already made before
            if (fs::equivalent(conf.links.at(n).source, conf.links.at(n).destination)) {
                std::println("confidant link: skipping '{}'", dest);
                continue;
            }
            
            // the files are not the same, just warn and skip
            std::println(std::cerr, "warning: destination '{}' already exists! skipping", dest);
            continue;
        }
        
        // check parent directory for write permission
        if (!util::perms_to_link(fs::path(dest))) {
            std::println(std::cerr, "error: no write permissions for '{}'", fs::path(dest).parent_path().string());
            continue;
        }
        
        // use create_directory_symlink for dirs
        if (linkType == confidant::config::linkType::directory) {
            
            if (src_stat.type() != fs::file_type::directory) {
                std::println(std::cerr, "error: '{}' is not a directory!", src);
                continue;
            }
            
            try {
                fs::create_directory_symlink(src, dest);
            } catch (const fs::filesystem_error& err) {
                std::println(std::cerr, "error: failed to create symlink at '{}'", dest);
                std::cout << err.what() << std::endl;
            }
            
        } else if (linkType == confidant::config::linkType::file) {
            
            try {
                fs::create_symlink(src, dest);
            } catch (const fs::filesystem_error& err) {
                std::println(std::cerr, "error: failed to create symlink at '{}'", dest);
                std::cout << err.what() << std::endl;
            }
            
        } else {
            // this should never happen, but hey; what can ya do.
            std::println(std::cerr, "error: link type for '{}' is not recognized, expected one of: file, directory", conf.links.at(n).name);
            continue;
        }
        
        // the file was linked
        std::println("confidant link: {} -> {}", src, dest);
        
    }
    
    return 0;

}
    
namespace debug {
void dumpConfig(const confidant::configuration& conf) {
    std::println("confidant.ucl:");
    std::println("  repository: ");
    std::println("    url: {}", conf.repo.url);
    std::println("  link:");
    int numlinks = conf.links.size();
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
    
    std::println("  link-from:");
    std::println("    from: {}", std::string(conf.linkFrom.from));
    std::println("    to: {}", std::string(conf.linkFrom.to));
    std::println("    items:");
    int numitems = conf.linkFrom.items.size();
    for (int n = 0; n < numitems; n++) {
        std::println("    - {}", conf.linkFrom.items.at(n));
    }
}

} // NOTE: end namespace confidant::debug

} // NOTE: end namespace confidant