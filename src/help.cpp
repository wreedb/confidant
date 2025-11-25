// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "i18n.hpp"
#include "config.hpp"
#include "help.hpp"
#include "fmt.hpp"
#include "msg.hpp"
#include "xdg.hpp"

namespace fg = fmt::fg;
namespace fs = std::filesystem;

namespace help {

namespace general {
    void help(std::string_view argz) {
        std::cout << fg::green(argz) << ":\n\n"
                  // i18n: "pal and confidant" is italicized
                  << "    " << _("your configuration") << " " << fmt::ital(_("pal and confidant")) << "\n\n"
                  << fg::magenta(_("actions")) << ":\n\n"
                  << "    " << fmt::ul("init") << "                " << _("initialize a repository") << "\n"
                  << "    " << fmt::ul("config") << "              " << _("view configuration") << "\n"
                  << "    " << fmt::ul("link") << "                " << _("create symlinks") << "\n"
                  << "    " << fmt::ul("help") << "                " << _("display help for subcommands") << "\n"
                  << "    " << fmt::ul("usage") << "               " << _("brief command-line usage info") << "\n"
                  << "    " << fmt::ul("version") << "             " << _("display version info") << "\n\n"
                  << fg::yellow(_("options")) << ":\n\n"
                  << "    -v, --verbose       " << _("output more information about actions taken") << "\n\n"
                  << "    -q, --quiet         " << _("suppress non-error messages") << "\n\n"
                  << "    -?, -h, --help      " << _("display this help") << "\n"
                  << std::endl;
    }
    void usage(std::string_view argz) {
        std::cout << _("usage") << ": " << fg::green(argz) << " [" << fmt::ul(_("action")) << "] [..." << _("options") << "]\n"
                  << fg::magenta(_("actions")) << ": "
                  << fmt::ul("init")    << ", "
                  << fmt::ul("config")  << ", "
                  << fmt::ul("link")    << ", "
                  << fmt::ul("help")    << ", "
                  << fmt::ul("usage")   << ", "
                  << fmt::ul("version") << "\n\n"
                  << _("see") << " " << "'confidant help <" << fmt::ul(_("action")) << ">' " << _("or") << " " << "'confidant <" << fmt::ul(_("action")) << "> -?'" << " "
                  << _("for more information\nabout a specific action and its' respective options.")
                  << std::endl;
    }
};

namespace init {
    void help(std::string_view argz) {
        std::cout << fg::green(argz) << " " << fmt::ul("init") << ":\n\n"
                  << "    " << _("initialize a confidant repository") << "\n\n"
                  << fg::blue(_("arguments")) << ":\n\n"
                  << "    " << fmt::ul(_("PATH")) << _("                ") << _("specify the path to initialize") << "\n"
                  << "                        " << _("default: current working directory") << "\n\n"
                  << fg::yellow(_("options")) << ":\n\n"
                  << "    -d, --dry-run       " << _("show what actions") << " " << fmt::ital(_("would")) << " " << _("be taken") << "\n\n"
                  << "    -v, --verbose       " << _("output more information about actions") << "\n\n"
                  << "    -q, --quiet         " << _("suppress non-error messages") << "\n\n"
                  << "    -?, -h, --help      " << _("display this help") << "\n"
                  << std::endl;
    }
};

namespace config {
    void help(std::string_view argz) {
        std::cout << fg::green(argz) << " " << fmt::ul("config") << ":\n\n"
                  << "    " << _("view configuration settings") << "\n\n"
                  << fg::magenta("actions") << ":\n\n"
                  << "    " << fmt::ul("dump") << _("                ") << _("display current configuration settings") << "\n"
                  << "    " << fmt::ul("get") << _("                 ") << _("find the value of a setting by name") << "\n\n"
                  << fg::yellow(_("options")) << ":\n\n"
                  << "    -v, --verbose       " << _("output more information about actions taken") << "\n\n"
                  << "    -q, --quiet         " << _("suppress non-error messages") << "\n\n"
                  << "    -?, -h, --help      " << _("display this help") << "\n\n"
                  << _("see 'confidant help config") << " <" << fmt::ul(_("action")) << ">' " << _("for more information about a specific command\nand its' respective options.")
                  << std::endl;
    }
namespace dump {
    void help(std::string_view argz) {
        std::cout << fg::green(argz) << " " << fmt::ul("config") << " " << fmt::ul("dump") << ":\n\n"
                  << "    " << _("display current configuration") << "\n\n"
                  << fg::yellow("options") << ":\n\n"
                  << "    -f, --file " << fmt::ul(_("PATH")) << _("     ") << _("specify a configuration file to operate on") << "\n"
                  << "                        " << _("default: <current directory>/confidant.ucl") << "\n\n"
                  << "    -g, --global        " << _("display the global configuration") << "\n\n"
                  << "    -j, --json          " << _("output configuration in JSON format") << "\n\n"
                  << "    -v, --verbose       " << _("output more information about actions taken") << "\n\n"
                  << "    -q, --quiet         " << _("suppress non-error messages") << "\n\n"
                  << "    -?, -h, --help      " << _("display this help") << "\n"
                  << std::endl;
    }
};
namespace get {
    void help(std::string_view argz) {
        std::cout << fg::green(argz) << " " << fmt::ul("config") << " " << fmt::ul("get") << ":\n\n"
                  << "    " << _("find a configuration value by name") << "\n\n"
                  << fg::blue(_("arguments")) << ":\n\n"
                  << "    " << fmt::ul(_("QUERY")) << _("               ") << _("the configuration setting to search for") << ",\n"
                  << "                        " << _("you may search for nested values by using") << "\n"
                  << "                        " << _("periods to traverse, e.g.: 'repository.url'") << "\n\n"
                  << fg::yellow(_("options")) << ":\n\n"
                  << "    -g, --global        " <<_("query the global configuration") << "\n\n"
                  << "    -v, --verbose       " <<_("output more information about actions taken") << "\n\n"
                  << "    -q, --quiet         " <<_("suppress non-error messages") << "\n\n"
                  << "    -?, -h, --help      " <<_("display this help") << "\n"
                  << std::endl;
    }
};
};

namespace link {
void help(std::string_view argz) {
    std::cout << fg::green(argz) << " " << fmt::ul("link") << ":\n\n"
              << "    " << _("apply symlinks from your configuration file") << "\n\n"
              << fg::yellow(_("options")) << ":\n\n"
              << "    -t, --tags " << fmt::ul("X,Y,Z") << _("    ")  << _("specify a set of tagged links to apply, separated by commas") << "\n\n"
              << "    -f, --file " << fmt::ul(_("PATH")) << _("     ") << _("specify the configuration file to operate on") << "\n"
              << "                        " << _("default: <current directory>/confidant.ucl") << "\n\n"
              << "    -d, --dry-run       " << _("show what actions") << " " << fmt::ital(_("would")) << " " << _("be taken") << "\n\n"
              << "    -v, --verbose       " << _("output more information about actions taken") << "\n\n"
              << "    -q, --quiet         " << _("suppress non-error messages") << "\n\n"
              << "    -?, -h, --help      " << _("display this help") << "\n"
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
        dest: ${xdg_config_home}/nvim/init.lua
    }

    # 'destdir' will reuse the basename of the source, resulting in
    # the full destination path being ${XDG_CONFIG_HOME}/fontconfig
    fontconfig {
        source: ${repo}/.config/fontconfig
        destdir: ${xdg_config_home}
        # by default, type is 'file'
        type: directory
    }
    
    bashrc {
        source: ${repo}/.bashrc
        dest: ${home}/.bashrc
    }
    
}

# 'templates' allows you to do linking with templated names
# note: unlike the 'links' section, templates do not care whether
# the item being linked is a file or directory, it is automatically
# determined
templates {
    config {
        source: ${repo}/.config/%{item}
        dest: ${xdg_config_home}/%{item}
        # each item will be substituted into the source and dest
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
                msg::error("failed to create directory {}", parent_dir.string());
                std::cerr << e.what() << std::endl;
            }
            msg::info("created directory {}", parent_dir.string());
        }
        
        if (!util::hasperms(parent_dir.string()))
            msg::fatal("no permission to write files to {}", parent_dir.string());
        
        if (fs::exists(p)) {
            msg::warn("file {} already exists, not overwriting", fmt::bolden(p));
            return;
        }
        
        std::ofstream handle((fs::path(p)));
        if (!handle.is_open())
            msg::fatal("failed to open {} for writing", fmt::bolden(p));
        
        handle << help::defaults::global_config();
        handle.close();
        
        msg::info("wrote configuration to file {}", fmt::bolden(util::unexpandhome(p)));
    }
    
    void write_local_config(std::string_view dir) {
        
        std::string outpath = std::format("{}/confidant.ucl", dir);
        
        if (fs::exists(outpath)) {
             msg::debug("file {} already exists, not overwriting.", fmt::bolden(outpath));
             return;
        }
        
        std::ofstream handle(outpath);
        
        if (!handle.is_open())
            msg::fatal("unable to open {} for writing", fmt::bolden(outpath));
        
        handle << help::defaults::local_config();
        handle.close();
        
        msg::debug("wrote configuration to file {}", fmt::bolden(fs::relative(outpath).string()));
        
    }
};

    
};