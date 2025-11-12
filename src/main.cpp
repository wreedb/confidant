// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>
#include <string>
#include <print>
#include <CLI/CLI.hpp>

#include "ucl.hpp"
#include "util.hpp"
#include "confidant.hpp"
#include "logging.hpp"
#include "config.hpp"

namespace logger = confidant::logging;

const static std::string version = std::string(PROJECT_VERSION);

void usageGeneral(const std::string& argz) {
    logger::info(argz, "options");
    std::println("{}commands{}:", logger::color(1), logger::color(0));
    std::println("\tinit: initialize a repository");
    std::println("\tlink: create symlinks");
    std::println("\tconfig: modify and view configuration");
    std::println("\thelp: display help for subcommands");
    std::println("\tversion: display version information");
    std::cout << std::endl;
    std::println("{}flags{}:", logger::color(1), logger::color(0));
    std::println("\t--verbose,-v: increase verbosity, can be repeated to further increase");
}

void usageInit(const std::string& argz) {
    logger::info(argz, "init [path]");
    std::println("initialize a repo at [path] {{.}}");
    std::println("\t--dry-run: show what actions would be taken");
    std::println("\t--verbose,-v: increase verbosity, can be repeated to further increase");
}

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

int main(const int argc, const char *argv[]) {

    std::string argz;

    if (argc > 0)
        argz = util::stripargz(argv[0]);
    else
        argz = std::string(PROJECT_NAME);

    if (argc >= 3 && std::string(argv[1]) == "help") {
        std::string topic = argv[2];
        if (topic == "init") {
            usageInit(argz);
            return 0;
        } else if (topic == "link") {
            usageLink(argz);
            return 0;
        } else if (topic == "version") {
            logger::info(argz, "version {}", version);
            return 0;
        } else if (topic == "config") {
            usageConfig(argz);
            return 0;
        } else {
            std::println(std::cerr, "{}: help topic '{}' is not known", argz, topic);
            return 1;
        }
    }
    
    if (argc == 2 && std::string(argv[1]) == "help") {
        usageGeneral(argz);
        return 0;
    }

    CLI::App args{"Your configuration \033[1;3mconfidant\033[m"};
    args.set_help_flag();
    args.set_version_flag();

    std::string optInit = ".";

    auto* cmdInit = args.add_subcommand("init", "Initialize configuration repository");

    cmdInit->add_option("path", optInit, "path to new repository")
           ->default_val(".");

    auto* cmdLink = args.add_subcommand("link", "create symlinks");

    auto* cmdVersion = args.add_subcommand("version", "display version info");

    auto* cmdConfig = args.add_subcommand("config", "modify and view configuration");
    auto* cmdConfig_dump = cmdConfig->add_subcommand("dump", "output the current configuration");
    std::string configPath = "confidant.ucl";
    [[maybe_unused]]auto* cmdConfig_dump_file = cmdConfig_dump->add_option("-f,--file", configPath, "specify configuration file to operate on");
    
    CLI11_PARSE(args, argc, argv);

    if (args.got_subcommand(cmdInit)) {
        std::println("Initializing repo at {}", optInit);
        return 0;
    } else if (args.got_subcommand(cmdLink)) {
        confidant::configuration conf = confidant::config::serialize("confidant.ucl");
        confidant::link(conf);
        confidant::linkfrom(conf);
        return 0;
    } else if (args.got_subcommand(cmdVersion)) {
        logger::info(argz, "version {}", version);
        return 0;
    } else if (args.got_subcommand(cmdConfig)) {
        if (cmdConfig->got_subcommand(cmdConfig_dump)) {
            confidant::configuration conf = confidant::config::serialize(configPath);
            logger::info(argz, "dumping configuration from {}", configPath);
            confidant::debug::dumpConfig(conf);
            return 0;
        } else {
            usageConfig(argz);
            return 1;
        }
    }

    return 0;

}