// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "config.hpp"
#include "help.hpp"
#include "logging.hpp"
#include "xdg.hpp"

namespace fg = confidant::logging::fg;
namespace fs = std::filesystem;
namespace logger = confidant::logging;

namespace help {

std::string tab(const int& amt) {
    std::string res = "";
    for (int x = 0; x < amt; x++) {
        res.append("\t");
    }
    return res;
}
    
namespace general {
    void help(std::string_view argz) {
        using logger::ul;
        std::cout << fg::green(argz) << ":\n\n"
                  << "    your configuration " << logger::ital("pal and confidant") << "\n\n"
                  << fg::magenta("actions") << ":\n\n"
                  << "    " << ul("init") << "                initialize a repository\n"
                  << "    " << ul("config") << "              modify and view configuration\n"
                  << "    " << ul("link") << "                create symlinks\n"
                  << "    " << ul("help") << "                display help for subcommands\n"
                  << "    " << ul("usage") << "               brief command-line usage info\n"
                  << "    " << ul("version") << "             display version info\n\n"
                  << fg::yellow("options") << ":\n\n"
                  << "    -v, --verbose       output more information about actions taken\n"
                  << std::endl;
    }
    void usage(std::string_view argz) {
        using logger::ul;
        std::cout << "usage: " << fg::green(argz) << " [" << ul("action") << "] [...options]\n"
                  << fg::magenta("actions") << ": "
                  << ul("init")    << ", "
                  << ul("config")  << ", "
                  << ul("link")    << ", "
                  << ul("help")    << ", "
                  << ul("usage")   << ", "
                  << ul("version") << "\n\n"
                  << "see 'confidant help <" << ul("action") << ">' for more information about a specific action\nand its' respective options."
                  << std::endl;
    }
};

namespace init {
    void help(std::string_view argz) {
        using logger::ul;
        std::cout << fg::green(argz) << " " << ul("init") << ":\n\n"
                  << "    initialize a confidant repository\n\n"
                  << fg::blue("arguments") << ":\n\n"
                  << "    "<< ul("PATH") <<"                specify the path to initialize\n"
                  << "                        default: current working directory\n\n"
                  << fg::yellow("options") << ":\n\n"
                  << "    -v, --verbose       output more information about actions taken\n"
                  << std::endl;
    }
};

namespace config {
    void help(std::string_view argz) {
        using logger::ul;
        std::cout << fg::green(argz) << " " << ul("config") << ":\n\n"
                  << "    view configuration settings\n\n"
                  << fg::magenta("actions") << ":\n\n"
                  << "    " << ul("dump") << "                display current configuration settings\n"
                  << "    " << ul("get") << "                 find the value of a setting by name\n\n"
                  << fg::yellow("options") << ":\n\n"
                  << "    -v, --verbose       output more information about actions taken\n\n"
                  << "see 'confidant help config <" << ul("action") << ">' for more information about a specific command\nand its' respective options."
                  << std::endl;
    }
namespace dump {
    void help(std::string_view argz) {
        using logger::ul;
        std::cout << fg::green(argz) << " " << ul("config") << " " << ul("dump") << ":\n\n"
                  << "    display current configuration\n\n"
                  << fg::yellow("options") << ":\n\n"
                  << "    -f, --file " << ul("PATH") << "     specify a configuration file to operate on\n"
                  << "                        default: <current directory>/confidant.ucl\n\n"
                  << "    -v, --verbose       output more information about actions taken\n"
                  << std::endl;
    }
};
namespace get {
    void help(std::string_view argz) {
        using logger::ul;
        std::cout << fg::green(argz) << " " << ul("config") << " " << ul("get") << ":\n\n"
                  << "    find a configuration value by name\n\n"
                  << fg::blue("arguments") << ":\n\n"
                  << "    " << ul("NAME") << "                the configuration setting to search for,\n"
                  << "                        you may search for nested values by using\n"
                  << "                        periods to traverse, e.g.: 'repository.url'\n\n"
                  << fg::yellow("options") << ":\n\n"
                  << "    -v, --verbose       output more information about actions taken\n"
                  << std::endl;
    }
};
};

namespace link {
void help(std::string_view argz) {
    using logger::ul;
    std::cout << fg::green(argz) << " " << ul("link") << ":\n\n"
              << "    apply symlinks from your configuration file\n\n"
              << fg::yellow("options") << ":\n\n"
              << "    -f, --file " << ul("PATH") << "     specify the configuration file to operate on\n"
              << "                        default: <current directory>/confidant.ucl\n\n"
              << "    -d, --dry-run       show what actions " << logger::ital("would") << " be taken\n\n"
              << "    -v, --verbose       output more information about actions taken\n"
              << std::endl;
}
    
}; // END help::link

namespace defaults {
    std::string global_config_path() {
        return std::format("{}/{}/config.ucl",
                           xdg::homes().at("XDG_CONFIG_HOME").string(),
                           PROJECT_NAME);
    }
    
    std::string local_config() {
        std::string s = R"(# this is an example (local) configuration file for confidant
#
# for more information about syntax, valid fields, and built-in variables,
# see the manual page: 'man 5 confidant.ucl'

/*
# metadata about the repository
repository {
    url: https://github.com/username/repository.git
}

# 'link' entries provide fine-grain explicit control over how
# your files are handled
links {
    nvim {
        source: ${repo}/.config/nvim/init.lua
        dest: ${XDG_CONFIG_HOME}/nvim/init.lua
    }

    # 'destdir' will reuse the basename of the source, resulting in
    # the full destination path being ${XDG_CONFIG_HOME}/fontconfig
    fontconfig {
        source: ${repo}/.config/fontconfig
        destdir: ${XDG_CONFIG_HOME}
        # by default, type is 'file'
        type: directory
    }
    
    bashrc {
        source: ${repo}/.bashrc
        dest: ${HOME}/.bashrc
    }
    
}

# 'templates' allows you to do linking with templated names
# note: unlike the 'links' section, templates do not care whether
# the item being linked is a file or directory, it is automatically
# determined
templates {
    config {
        source: ${repo}/.config/%{item}
        dest: ${XDG_CONFIG_HOME}/%{item}
        # each item will be substituted into the 'from' and 'to'
        # values, and then will be symlinked accordingly
        items: [
            kitty/kitty.conf
            fish/conf.d
            fish/config.fish
        ]
    }
    # the resulting links would be:
    # ${repo}/.config/kitty/kitty.conf -> ${XDG_CONFIG_HOME}/kitty/kitty.conf
    # ${repo}/.config/fish/conf.d      -> ${XDG_CONFIG_HOME}/fish/conf.d
    # ${repo}/.config/fish/config.fish -> ${XDG_CONFIG_HOME}/fish/config.fish
}
*/)";
        return s;
    }
    
    std::string global_config() {
        return R"(
# default global configuration for confidant

# by default, confidant will attempt to create parent directories if they don't exist
# to disable, set this to false.
create-directories: true

# the default verbosity level, before command-line options are parsed.
# valid values:
# [quiet, normal, info, debug, trace] or [0, 1, 2, 3, 4]
log-level: normal

# whether or not to use ansi escape sequences for colored and formatted 
# command-line output, default is true. The `NO_COLOR` environment variable 
# being present as anything other than an empty string will override a true
# value here.
color: true
)";

    }
    
    void write_global_config(std::string_view p) {
        fs::path parent_dir = fs::path(p).parent_path();
        if (!fs::exists(parent_dir)) {
            try {
                fs::create_directories(parent_dir);
            } catch (const fs::filesystem_error& e) {
                logger::error("failed to create directory {}", parent_dir.string());
                std::cerr << e.what() << std::endl;
            }
            logger::info("created directory {}", parent_dir.string());
        }
        
        if (!util::hasperms(parent_dir.string()))
            logger::fatal(1, "no permission to write files to {}", parent_dir.string());
        
        if (fs::exists(p)) {
            logger::warn("file {} already exists, not overwriting", logger::bolden(p));
            return;
        }
        
        std::ofstream handle((fs::path(p)));
        if (!handle.is_open())
            logger::fatal(1, "failed to open {} for writing", logger::bolden(p));
        
        handle << help::defaults::global_config();
        handle.close();
        
        logger::info("wrote configuration to file {}", logger::bolden(util::unexpandhome(p)));
    }
    
    void write_local_config(std::string_view dir) {
        
        std::string outpath = std::format("{}/confidant.ucl", dir);
        
        if (fs::exists(outpath)) {
             logger::warn("file {} already exists, not overwriting.", logger::bolden(outpath));
             return;
        }
        
        std::ofstream handle(outpath);
        
        if (!handle.is_open())
            logger::fatal(1, "unable to open {} for writing", logger::bolden(outpath));
        
        handle << help::defaults::local_config();
        handle.close();
        
        logger::info("wrote configuration to file {}", logger::bolden(fs::relative(outpath).string()));
        
    }
};

    
};