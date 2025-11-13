// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>
#include <string>
#include <print>
#include <algorithm>
#include <CLI/CLI.hpp>

#include "ucl.hpp"
#include "util.hpp"
#include "confidant.hpp"
#include "logging.hpp"
#include "config.hpp"
#include "help.hpp"

namespace logger = confidant::logging;

const static std::string version = std::string(PROJECT_VERSION);

void usageConfig(const std::string& argz) {
    logger::info(argz, "config <action> [...options]");
    std::println("view and modify configuration. default values shown in {{ }} braces");
    std::println("{}commands{}:", logger::color(1), logger::color(0));
    std::println("\t{}dump{}: output the current configuration", logger::color(1), logger::color(0));
    std::println("\t-f,--file {}path{}: specify configuration file path {{confidant.ucl}}", logger::color(1), logger::color(0));
}

void usageLink(const std::string& argz) {
    logger::info(argz, "link [--dry-run,--verbose]");
    std::println("create symlinks");
    std::println("\t--dry-run: show what actions would be taken");
    std::println("\t--verbose: show more info about actions taken");
}

int main(int argc, char *argv[]) {

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
        } else if (topic == "link-from") {
            help::link_from::help(argz);
            return 0;
        } else if (topic == "version") {
            logger::info(argz, "version {}", version);
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
            std::println(std::cerr, "{}: help topic '{}' is not known", argz, topic);
            return 1;
        }
    }
    
    if (argc == 2 && std::string(argv[1]) == "help") {
        // usageGeneral(argz);
        help::general::help(argz);
        return 0;
    }

    CLI::App args{"Your configuration \033[1;3mconfidant\033[m"};
    argv = args.ensure_utf8(argv);
    args.set_help_flag();
    args.set_version_flag();

    int verbosity = ansi::verbosity::normal;
    
    std::string optInit = ".";
    std::string optConfig_get_name;
    std::string optConfig_dump_file = "confidant.ucl";
    
    std::string optLink_file = "confidant.ucl";
    std::string optLinkFrom_file = "confidant.ucl";
    
    bool optLink_dryrun = false;
    bool optLinkFrom_dryrun = false;
    bool optInit_dryrun = false;
    bool optHelp = false;
    bool optVersion = false;

    [[maybe_unused]]auto* flagVerbosity = args.add_flag("-v,--verbose", verbosity);
    
    auto* cmdInit = args.add_subcommand("init", "Initialize configuration repository");

    cmdInit->add_option("path", optInit);
    cmdInit->add_flag("-d,--dry-run", optInit_dryrun);
    
    [[maybe_unused]]auto* flagVersion = args.add_flag("-V,--version", optVersion)->default_val(false);
    [[maybe_unused]]auto* flagHelp = args.add_flag("-h,--help", optHelp)->default_val(false);
    
    auto* cmdLink = args.add_subcommand("link");
    auto* cmdLinkFrom = args.add_subcommand("link-from");

    [[maybe_unused]]auto* cmdLinkFrom_dryrun = cmdLinkFrom->add_flag("-d,--dry-run", optLinkFrom_dryrun);
    [[maybe_unused]]auto* cmdLink_dryrun = cmdLink->add_flag("-d,--dry-run", optLinkFrom_dryrun);
 
    
    auto* cmdVersion = args.add_subcommand("version");
    auto* cmdUsage = args.add_subcommand("usage");

    auto* cmdConfig = args.add_subcommand("config");
    auto* cmdConfig_dump = cmdConfig->add_subcommand("dump");
    auto* cmdConfig_get = cmdConfig->add_subcommand("get");

    [[maybe_unused]]auto* cmdConfig_get_name = cmdConfig_get->add_option("name", optConfig_get_name);
    
    std::string configPath = "confidant.ucl";
    [[maybe_unused]]auto* cmdConfig_dump_file = cmdConfig_dump->add_option("-f,--file", optConfig_dump_file);
    
    [[maybe_unused]]auto* cmdLink_file = cmdLink->add_option("-f,--file", optLink_file);
    [[maybe_unused]]auto* cmdLinkFrom_file = cmdLinkFrom->add_option("-f,--file", optLinkFrom_file);
    
    CLI11_PARSE(args, argc, argv);
    
    // apply verbosity flags to loglevel
    ansi::loglevel = std::min(int(ansi::verbosity::normal) + verbosity, int(ansi::verbosity::trace));
    
    if (optVersion) {
        logger::info(argz, "version {}", PROJECT_VERSION);
        return 0;
    }
    
    if (optHelp) {
        help::general::help(argz);
        return 0;
    }
    
    if (args.got_subcommand(cmdUsage)) {
        help::general::usage(argz);
        return 0;
    } else if (args.got_subcommand(cmdInit)) {
        if (optInit_dryrun) {
            std::println("writing default configuration to: {}/confidant.ucl", optInit);
            return 0;
        } else {
            help::defaults::write_local_config(optInit);
            return 0;
        }
    
    } else if (args.got_subcommand(cmdLink)) {
        return confidant::link(confidant::config::serialize(optLink_file), optLink_dryrun);
        
    } else if (args.got_subcommand(cmdLinkFrom)) {
        return confidant::linkfrom(confidant::config::serialize(optLinkFrom_file), optLinkFrom_dryrun);
    
    } else if (args.got_subcommand(cmdVersion)) {
        logger::info(argz, "version {}", version);
        return 0;
    
    } else if (args.got_subcommand(cmdConfig)) {
        if (cmdConfig->got_subcommand(cmdConfig_dump)) {
            confidant::configuration conf = confidant::config::serialize(optConfig_dump_file);
            logger::info(argz, "dumping configuration from {}", optConfig_dump_file);
            confidant::debug::dumpConfig(conf);
            return 0;
        } else {
            logger::error(argz, "unknown subcommand");
            help::config::help(argz);
            return 1;
        }
    }

    return 0;

}