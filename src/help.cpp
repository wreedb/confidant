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

#define sv  std::string_view
#define out std::cout
#define err std::cerr
#define nl  std::endl
#define fmt std::format

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
    void help(sv argz) {
        out << logger::color(1) << argz << logger::color(22) << ":\n\n"
            << "    your configuration " << logger::color(3) << "pal and confidant" << logger::color(23) << "\n\n"
            << "actions:\n\n"
            << "    init                initialize a repository\n"
            << "    config              modify and view configuration\n"
            << "    link                create symlinks\n"
            << "    link-from           create templated symlinks\n"
            << "    help                display help for subcommands\n"
            << "    usage               brief command-line usage info\n"
            << "    version             display version info\n\n"
            << "options:\n\n"
            << "    -v, --verbose       output more information about actions taken,\n"
            << "                        may be repeated to increase verbosity level\n"
            << nl;
    }
    void usage(sv argz) {
        out << "usage: " << argz << " [action] [...options]\n"
            << "actions: init, config, link, link-from, help, usage, version\n\n"
            << "see 'confidant help <command>' for more information about a specific command\nand its' respective options.\n"
            << nl;
    }
};

namespace init {
    void help(std::string_view argz) {
        out << logger::color(1) << argz << logger::color(0) << " init:\n\n"
            << "    initialize a confidant repository\n\n"
            << "arguments:\n\n"
            << "    PATH                specify the path to initialize\n"
            << "                        default: current working directory\n\n"
            << "options:\n\n"
            << "    -f, --file          specify the repository config file to read\n"
            << "                        default: <repo>/confidant.ucl\n\n"
            << "    -v, --verbose       output more information about actions taken,\n"
            << "                        may be repeated to increase verbosity level\n"
            << nl;
    }
};

namespace config {
    void help(std::string_view argz) {
        out << logger::color(1) << argz << logger::color(0) << " config:\n\n"
            << "    view and modify configuration\n\n"
            << "actions:\n\n"
            << "    dump                display current configuration settings\n"
            << "    get                 find the value of a setting by name\n\n"
            << "options:\n\n"
            << "    -v, --verbose       output more information about actions taken,\n"
            << "                        may be repeated to increase verbosity level\n\n"
            << "see 'confidant help config <action>' for more information about a specific command\nand its' respective options."
            << nl;
    }
namespace dump {
    void help(std::string_view argz) {
        out << logger::color(1) << argz << logger::color(0) << " config dump:\n\n"
            << "    display current configuration\n\n"
            << "options:\n\n"
            << "    -f, --file PATH     specify a configuration file to operate on\n"
            << "                        default: <repo>/confidant.ucl\n\n"
            << "    -v, --verbose       output more information about actions taken,\n"
            << "                        may be repeated to increase verbosity level\n\n"
            << nl;
    }
};
namespace get {
    void help(std::string_view argz) {
        out << logger::color(1) << argz << logger::color(0) << " config get:\n\n"
            << "    find a configuration value by name\n\n"
            << "arguments:\n\n"
            << "    NAME                the configuration setting to search for\n\n"
            << "options:\n\n"
            << "    -s, --section NAME  specify a configuration section to search in\n"
            << "                        default: top level of configuration file\n\n"
            << "    -v, --verbose       output more information about actions taken,\n"
            << "                        may be repeated to increase verbosity level\n\n"
            << nl;
    }
};
};

namespace link {
void help(std::string_view argz) {
    out << logger::color(1) << argz << logger::color(0) << " link:\n\n"
        << "    apply symlinks from the" << logger::color(1) << " link " << logger::color(0) << "section of your configuration\n\n"
        << "options:\n\n"
        << "    -f, --file PATH     specify the configuration file to operate on\n"
        << "                        default: <repo>/confidant.ucl\n\n"
        << "    -n, --name NAME     apply link for only provided entry name\n\n"
        << "    -d, --dry-run       show what actions" << logger::color(3) << " would " << logger::color(0) << "be taken\n\n"
        << "    -v, --verbose       output more information about actions taken,\n"
        << "                        may be repeated to increase verbosity level\n\n"
        << nl;
}
    
}; // END help::link

namespace link_from {
    void help(std::string_view argz) {
        out << logger::color(1) << argz << logger::color(0) << " link-from:\n\n"
            << "    apply symlinks from the" << logger::color(1) << " link-from " << logger::color(0) << "section of your configuration\n\n"
            << "options:\n\n"
            << "    -f, --file PATH     specify the configuration file to operate on\n"
            << "                        default: <repo>/confidant.ucl\n\n"
            << "    -d, --dry-run       show what actions" << logger::color(3) << " would " << logger::color(0) << "be taken\n\n"
            << "    -v, --verbose       output more information about actions taken,\n"
            << "                        may be repeated to increase verbosity level\n\n"
            << nl;
    }
}; // END help::link_from

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
link {
    nvim {
        source: ${REPO}/.config/nvim/init.lua
        dest: ${XDG_CONFIG_HOME}/nvim/init.lua
    }

    # 'destdir' will reuse the basename of the source, resulting in
    # the full destination path being ${XDG_CONFIG_HOME}/fontconfig
    fontconfig {
        source: ${REPO}/.config/fontconfig
        destdir: ${XDG_CONFIG_HOME}
        # by default, type is 'file'
        type: directory
    }
    
    bashrc {
        source: ${REPO}/.bashrc
        dest: ${HOME}/.bashrc
    }
    
}

# 'link-from' allows you to do linking with templated names
# note: unlike link, link-from does not care whether the item
# being linked is a file or directory.
link-from {
    from: ${REPO}/.config/%{item}
    to: ${XDG_CONFIG_HOME}/%{item}
    # each item will be substituted into the 'from' and 'to'
    # values, and then will be symlinked accordingly
    items: [
        kitty/kitty.conf
        fish/conf.d
        fish/config.fish
    ]
    # the resulting links would be:
    # ${REPO}/.config/kitty/kitty.conf -> ${XDG_CONFIG_HOME}/kitty/kitty.conf
    # ${REPO}/.config/fish/conf.d      -> ${XDG_CONFIG_HOME}/fish/conf.d
    # ${REPO}/.config/fish/config.fish -> ${XDG_CONFIG_HOME}/fish/config.fish
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

# the default verbosity level, before command-line options are parsed
# valid values:
# [quiet, normal, info, debug, trace] or [0, 1, 2, 3, 4]
log-level: normal
)";

    }
    
    void write_global_config(std::string_view p) {
        fs::path parent_dir = fs::path(p).parent_path();
        if (!fs::exists(parent_dir)) {
            try {
                fs::create_directories(parent_dir);
            } catch (const fs::filesystem_error& e) {
                logger::error(PROJECT_NAME, "failed to create directory {}", parent_dir.string());
                std::cerr << e.what() << std::endl;
            }
            logger::info(PROJECT_NAME, "created directory {}", parent_dir.string());
        }
        
        if (!util::perms_to_write(parent_dir))
            logger::fatal(PROJECT_NAME, 1, "no permission to write files to {}", parent_dir.string());
        
        if (fs::exists(p)) {
            logger::warn(PROJECT_NAME, "file {} already exists, not overwriting", p);
            std::exit(0);
        }
        
        std::ofstream handle((fs::path(p)));
        if (!handle.is_open())
            logger::fatal(PROJECT_NAME, 1, "failed to open {} for writing", p);
        
        handle << help::defaults::global_config();
        handle.close();
        
        logger::info(PROJECT_NAME, "write configuration to file {}", util::unexpandhome(p));
    }
    
    void write_local_config(std::string_view dir) {
        
        std::string outpath = std::format("{}/confidant.ucl", dir);
        
        if (fs::exists(outpath)) {
             logger::warn(PROJECT_NAME, "file '{}' already exists, not overwriting.", outpath);
             std::exit(1);
        }
        
        std::ofstream handle(outpath);
        
        if (!handle.is_open())
            logger::fatal(PROJECT_NAME, 1, "unable to open '{}' for writing", outpath);
        
        handle << help::defaults::local_config();
        handle.close();
        
        logger::info(PROJECT_NAME, "wrote configuration to file {}", fs::relative(outpath).string());
        
    }
};

    
};