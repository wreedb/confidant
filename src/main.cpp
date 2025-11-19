// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>
#include <print>
#include <filesystem>
#include <CLI/CLI.hpp>

#include "settings.hpp"
#include "settings/global.hpp"
#include "ucl.hpp"
#include "parse.hpp"
#include "util.hpp"
#include "confidant.hpp"
#include "logging.hpp"
#include "help.hpp"
#include "options.hpp"
#include "config.hpp"

namespace say = confidant::logging;
namespace fs = std::filesystem;



const static bool initusecolor = util::usecolorp();

int main(int argc, char *argv[]) {

    if (!initusecolor) confidant::config::global::color = false;
    
    std::string argz;

    if (argc > 0)
        argz = util::stripargz(argv[0]);
    else
        argz = std::string(PROJECT_NAME);

    if (argc >= 3 && std::string(argv[1]) == "help") {
        
        std::string topic = argv[2];
        
        if (topic == "init") {
            help::init::help(argz);
            return 0;
            
        } else if (topic == "link") {
            help::link::help(argz);
            return 0;
            
        } else if (topic == "version") {
            std::println("{}: version {}", say::fg::yellow(PROJECT_NAME), PROJECT_VERSION);
            return 0;
            
        } else if (topic == "config") {
            if (argc >= 4) {
        
                if (std::string(argv[3]) == "dump") {
                    help::config::dump::help(argz);
                    return 0;
                }
                
                if (std::string(argv[3]) == "get") {
                    help::config::get::help(argz);
                    return 0;
                }
        
            }
            help::config::help(argz);
            return 0;
        } else {
            std::println("help topic {} is not recognized", say::bolden(topic));
            return 1;
        }
    }
    
    if (argc == 2 && std::string(argv[1]) == "help") {
        help::general::help(argz);
        return 0;
    }
    
    // before setting up the argument parser, read the global config so the settings apply.
    if (!fs::exists(help::defaults::global_config_path()))
        help::defaults::write_global_config(help::defaults::global_config_path());
    
    confidant::settings settings = confidant::config::global::serialize(help::defaults::global_config_path());
    
    CLI::App args{"your configuration confidant"};
    argv = args.ensure_utf8(argv);
    args.set_help_flag();
    args.set_version_flag();

    confidant::config::global::loglevel = settings.loglevel;
    confidant::config::global::color = settings.color;
    confidant::config::global::createdirs = settings.createdirs;
    
    args.add_flag("-v,--verbose", options::verbosity);
    args.add_flag("-V,--version", options::version);
    args.add_flag("-h,--help",    options::help);
    args.add_flag("-u,--usage",   options::usage);
    args.add_flag("-q,--quiet",   options::quiet);
    
    auto* cmdVersion = args.add_subcommand("version");
    auto* cmdUsage   = args.add_subcommand("usage");
    
    auto* cmdInit = args.add_subcommand("init", "initialize a confidant repository");
    cmdInit->add_option("path",       options::init::path);
    cmdInit->add_flag("-d,--dry-run", options::init::dryrun);
    cmdInit->add_flag("-h,--help",    options::init::help);
    cmdInit->add_flag("-q,--quiet",   options::init::quiet);
    
    auto* cmdLink = args.add_subcommand("link", "apply symlinks");
    cmdLink->add_flag("-d,--dry-run", options::link::dryrun);
    cmdLink->add_flag("-h,--help",    options::link::help);
    cmdLink->add_flag("-v,--verbose", options::link::verbosity);
    cmdLink->add_option("-f,--file",  options::link::file);
    cmdLink->add_option("-q,--quiet",  options::link::quiet);
    
    auto* cmdConfig = args.add_subcommand("config");
    cmdConfig->add_flag("-v,--verbose", options::config::verbosity);
    cmdConfig->add_flag("-h,--help",    options::config::help);
    cmdConfig->add_flag("-q,--quiet",    options::config::quiet);
        
    auto* cmdConfig_dump = cmdConfig->add_subcommand("dump");
    cmdConfig_dump->add_flag("-h,--help",    options::config::dump::help);
    cmdConfig_dump->add_flag("-g,--global",  options::config::dump::global);
    cmdConfig_dump->add_flag("-v,--verbose", options::config::dump::verbosity);
    cmdConfig_dump->add_flag("-q,--quiet",   options::config::dump::quiet);
    cmdConfig_dump->add_option("-f,--file",  options::config::dump::file);
    
    auto* cmdConfig_get = cmdConfig->add_subcommand("get");
    cmdConfig_get->add_flag("-h,--help",    options::config::get::help);
    cmdConfig_get->add_flag("-v,--verbose", options::config::get::verbosity);
    cmdConfig_get->add_flag("-q,--quiet",   options::config::get::quiet);
    cmdConfig_get->add_option("-f,--file",  options::config::get::file);
    cmdConfig_get->add_option("name",       options::config::get::name);
    
    CLI11_PARSE(args, argc, argv);
    
    // if --quiet, always be quiet, otherwise follow config setting;
    // if --verbose, set to max verbosity, otherwise follow config setting;
    
    if (options::quiet ||
        options::config::quiet ||
        options::init::quiet ||
        options::link::quiet ||
        options::config::dump::quiet ||
        options::config::get::quiet) {
        confidant::config::global::loglevel = verbose::quiet;
    } else {
        int sumverbosity =
            ( options::verbosity
            + options::config::verbosity
            + options::config::get::verbosity
            + options::config::dump::verbosity
            + options::init::verbosity
            + options::link::verbosity);
        
        if (sumverbosity > 0)
            confidant::config::global::loglevel = verbose::trace;
    }
    
    if (options::version) {
        std::println("{}: version {}", say::fg::yellow(PROJECT_NAME), PROJECT_VERSION);
        return 0;
    }
    
    if (options::help) {
        help::general::help(argz);
        return 0;
    }
    
    if (args.got_subcommand(cmdUsage) || options::usage) {
        help::general::usage(argz);
        return 0;
        
    } else if (args.got_subcommand(cmdInit)) {
        if (options::init::help) {
            help::init::help(argz);
            return 0;
        }
        
        if (options::init::dryrun) {
            std::println("writing default configuration to: {}/confidant.ucl", options::init::path);
            return 0;
        
        } else {
            help::defaults::write_local_config(options::init::path);
            return 0;
        }
    
    } else if (args.got_subcommand(cmdLink)) {
        if (options::link::help) {
            help::link::help(argz);
            return 0;
        }
        
        confidant::configuration conf = confidant::config::local::serialize(options::link::file, settings);
        
        int ecode_link = confidant::link(conf, options::link::dryrun);
        int ecode_linktemplate = confidant::linktemplate(conf, options::link::dryrun);
        if (ecode_link + ecode_linktemplate >= 1)
            return 1;
        else
            return 0;
        
    } else if (args.got_subcommand(cmdVersion)) {
        std::println("{}: version {}", say::fg::yellow(PROJECT_NAME), PROJECT_VERSION);
        return 0;
    
    } else if (args.got_subcommand(cmdConfig)) {
        if (options::config::help) {
            help::config::help(argz);
            return 0;
        }
        
        if (cmdConfig->got_subcommand(cmdConfig_get)) {
            if (options::config::get::help) {
                help::config::get::help(argz);
                return 0;
            }
            auto conf = confidant::config::local::serialize(options::config::get::file, settings);
            optional<confidant::config::ConfigValue> results = confidant::config::get(conf, options::config::get::name);
            if (!results) {
                say::error("failed to find configuration value for {}",
                    say::bolden(options::config::get::name));
                return 1;
            }
            cout << confidant::config::formatconfigvalue(results.value()) << std::endl;
            return 0;
        }
        
        if (cmdConfig->got_subcommand(cmdConfig_dump)) {
            if (options::config::dump::help) {
                help::config::dump::help(argz);
                return 0;
            }
            
            if (options::config::dump::global) {
                confidant::debug::global::dumpConfig(settings);
                return 0;
            }
            
            say::pretty("dumping {}", util::unexpandhome(options::config::dump::file));
            confidant::debug::dumpConfig
                (confidant::config::local::serialize(options::config::dump::file, settings));
            return 0;
        
        } else {
            say::error("unknown subcommand");
            help::config::help(argz);
            return 1;
        }
    
    }
    help::general::usage(argz);
    return 0;

}