// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <string>
#include <utility>

#include "settings/global.hpp"
#include "util.hpp"
#include "fmt.hpp"
#include "msg.hpp"
#include "settings/local.hpp"

namespace fs = std::filesystem;

namespace confidant {
    
    namespace actions {
        
        namespace link {
        
            int linktemplate(const confidant::config::local::settings& conf, const confidant::config::global::settings& globals, const std::vector<std::string_view>& tags, bool dry) {
                int processedTemplates = 0;
                for (auto& tmpl : conf.templates) {
              
                    int processedItems = 0;
                    int numitems = tmpl.items.size();
                    
                    if (!tmpl.tag.empty() && tags.size() == 0) continue;
                    if (!tmpl.tag.empty() && tags.size() > 0) {
                        bool matches = std::find(tags.begin(), tags.end(), tmpl.tag) != tags.end();
                        if (!matches) continue;
                    }
                    
                    for (int n = 0; n < numitems; n++) {
                        
                        std::string sourcestr = util::substitute(tmpl.source.string() , tmpl.items.at(n));
                        std::string deststr   = util::substitute(tmpl.destination.string(), tmpl.items.at(n));
                        
                        fs::path sourcepath = fs::path(sourcestr);
                        fs::path destpath = fs::path(deststr);
                        
                        std::string usourcestr = fs::relative(fs::path(sourcestr)).string();
                        std::string udeststr = util::unexpandhome(deststr);
                        
                        if (!fs::exists(sourcepath)) {
                            msg::error("source file {} does not exist!",
                                fmt::bolden(usourcestr));
                            continue;
                        }
                        
                        if (fs::exists(destpath)) {
                            if (fs::equivalent(sourcepath, destpath)) {
                                msg::extra("skipping {}, already linked", fmt::bolden(udeststr));
                                continue;
                            }
                            msg::warn("destination {} already exists, skipping",
                                fmt::bolden(udeststr));
                            continue;
                        
                        } else if (fs::is_symlink(destpath) && !fs::exists(destpath)) {
                            // it's a broken symlink, remove it
                            if (!dry) {
                                try {
                                    fs::remove(destpath);
                                } catch (const fs::filesystem_error& e) {
                                    msg::error("failed to remove broken symlink at {}", fmt::bolden(deststr));
                                    std::cout << e.what() << std::endl;
                                    std::exit(1);
                                }
                            } else {
                                msg::warnextra("removing broken symlink at {}", fmt::bolden(deststr));
                            }
                        }
                        
                        if (!fs::exists(destpath.parent_path())) {
                            if (!globals.createdirs) {
                                msg::warn("parent directory for {} doesn't exist, skipping", fmt::bolden(udeststr));
                                continue;
                            } else {
                                if (!dry) {
                                    try {
                                        fs::create_directories(destpath.parent_path());
                                    } catch (const fs::filesystem_error& err) {
                                        msg::error("failed to create directory {}",
                                            fmt::bolden(util::unexpandhome(destpath.parent_path().string())));
                                        std::cout << err.what() << std::endl;
                                        return 1;
                                    }
                                }
                                msg::extra("created directory {}",
                                    fmt::bolden(util::unexpandhome(destpath.parent_path().string())));
                            }
                        }
                        
                        if (!util::hasperms(deststr) && !dry) {
                            msg::error("no write permissions for directory {}",
                                fmt::bolden(util::unexpandhome(destpath.parent_path().string())));
                            continue;
                        }
                        
                        if (fs::status(sourcepath).type() == fs::file_type::directory) {
                            if (!dry) {
                                try {
                                    // create the link
                                    fs::create_directory_symlink(sourcepath, destpath);
                                } catch (const fs::filesystem_error& err) {
                                    msg::error("failed to create symlink at {}", fmt::bolden(udeststr));
                                    std::cout << err.what() << std::endl;
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
                                    msg::error("failed to create symlink at {}", fmt::bolden(udeststr));
                                    std::cout << err.what() << std::endl;
                                    return 1;
                                }
                            }
                            processedItems++;
                        }
                        // show message regardless for dry-runs
                        msg::pretty("linked {}", fmt::bolden(udeststr));
                    }
                    // show *something* when nothing happens
                    if (processedItems == 0)
                        msg::pretty("template {} required no links", fmt::bolden(tmpl.name));
                    msg::trace("processed {} items", processedItems);
                    processedTemplates++;
                }
                msg::trace("processed {} templates", processedTemplates);
                
                return 0;
            }
            
            int linknormal(const config::local::settings& conf, const config::global::settings& globals, const std::vector<std::string_view>& tags, bool dry) {
                using util::unexpandhome;
                int numlinks = conf.links.size();
                int linksdone = 0;
                
                for (int n = 0; n < numlinks; n++) {
                    
                    std::string name       = conf.links.at(n).name;
                    std::string tag        = conf.links.at(n).tag;
                    fs::path sourcepath    = conf.links.at(n).source;
                    fs::path destpath      = conf.links.at(n).destination;
                    std::string sourcestr  = sourcepath.string();
                    std::string deststr    = destpath.string();
                    std::string usourcestr = fs::relative(sourcepath).string();
                    std::string udeststr   = util::unexpandhome(deststr);
                    
                    config::local::linktype lt = conf.links.at(n).type;
                
                    if (!tag.empty() && tags.size() == 0) continue;
                    
                    // skip entry when tag wasn't specified
                    if (!tag.empty() && tags.size() > 0) {
                        bool matches = std::find(tags.begin(), tags.end(), tag) != tags.end();
                        if (!matches) continue;
                    }
                    
                    // skip if the source file doesn't exist
                    if (!fs::exists(sourcepath)) {
                        msg::error("source file {} does not exist!", fmt::bolden(usourcestr));
                        continue;
                    }
                    // only after checking the file exists
                    fs::file_status sourcefstat = fs::status(sourcepath);
                    
                    // check if the destination exists
                    if (fs::exists(destpath)) {
                        // if the source and dest are the same file, e.g. the link was (likely) already created by us
                        if (fs::equivalent(sourcepath, destpath)) {
                            msg::extra("skipping {}, already linked", fmt::bolden(udeststr));
                            continue;
                        }
                        
                        // the destination already exists, and isn't identical to our source; we do nothing and skip
                        msg::warn("destination {} already exists and is not identical to source, skipping",
                            fmt::bolden(udeststr));
                        continue;
                        
                    } else if (fs::is_symlink(destpath) && !fs::exists(destpath)) {
                        // it's a broken symlink, remove it
                        if (!dry) {
                            try {
                                fs::remove(destpath);
                            } catch (const fs::filesystem_error& e) {
                                msg::error("failed to remove broken symlink at {}", fmt::bolden(deststr));
                                std::cout << e.what() << std::endl;
                                std::exit(1);
                            }
                        } else {
                            msg::extra("removing broken symlink at {}", fmt::bolden(deststr));
                        }
                    }
                    
                    if (!fs::exists(destpath.parent_path())) {
                        if (!globals.createdirs) {
                            // parent path doesn't exist and settings to create dirs is off
                            msg::warn("parent directory for {} does not exist, skipping",
                                fmt::bolden(name));
                            continue;
                        } else {
                            // create dirs unless we are doing a dry run
                            if (!dry) {
                                try {
                                    fs::create_directories(destpath.parent_path());
                                } catch (const fs::filesystem_error& err) {
                                    msg::error("failed to create directory {}",
                                        fmt::bolden(unexpandhome(destpath.parent_path().string())));
                                    std::cout << err.what() << "\n";
                                    return 1;
                                }
                            }
                            // display extra message regardless, for dry-run verbose
                            msg::extra("create directory {}",
                                fmt::bolden(unexpandhome(destpath.parent_path().string())));
                        }
                    }
                    
                    // check parent directory for write permission
                    if (!util::hasperms(deststr) && !dry) {
                        msg::error("no write permissions for {}",
                            fmt::bolden(unexpandhome(destpath.parent_path().string())));
                        // TODO: add strict setting, continue if true, return 1 if false
                        continue;
                    }
                    
                    // use create_directory_symlink for dirs because apparenty
                    // some inferior operating systems treat directory symlinks
                    // differently to file symlinks
                    if (lt == config::local::linktype::directory) {
                        
                        if (sourcefstat.type() != fs::file_type::directory) {
                            // they specified directory, but the source is not a directory
                            msg::error("link {} source {} is not a directory",
                                fmt::bolden(name),
                                fmt::ital(usourcestr));
                            // TODO: strict return 1
                            continue;
                        }
                        if (!dry) {
                            try {
                                // create the link
                                fs::create_directory_symlink(sourcepath, destpath);
                            } catch (const fs::filesystem_error& err) {
                                msg::error("failed to create symlink for {} at {}",
                                   fmt::bolden(name),
                                   fmt::ital(udeststr));
                                std::cout << err.what() << std::endl;
                                // TODO: continue when strict == false
                                return 1;
                            }
                        }
                        linksdone++;
                        
                    } else if (lt == config::local::linktype::file) {
                        if (!dry) {
                            try {
                                // create the link
                                fs::create_symlink(sourcepath, destpath);
                            } catch (const fs::filesystem_error& err) {
                                 msg::error("failed to create symlink for {} at {}",
                                    fmt::bolden(name),
                                    fmt::ital(udeststr));
                                 std::cout << err.what() << std::endl;
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
                    msg::pretty("linked {}", udeststr);
                }
                // show *something* when nothing happens at least
                if (linksdone == 0)
                    msg::pretty("no links were needed");
                msg::trace("created {} normal links", fmt::bolden(std::to_string(linksdone)));
                return 0;
        
            }

    };
  };  
};