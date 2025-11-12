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
#include "config.hpp"

const static std::string version = std::string(PROJECT_VERSION);

void usageGeneral() {
    std::println("confidant:");
    std::println("\tinit: initialize a repository");
    std::println("\tlink: create symlinks");
    std::println("\tconfig: modify and view configuration");
    std::println("\thelp: display help for subcommands");
}

void usageInit() {
    std::println("confidant init [PATH]: default: \".\"");
    std::println("Initialize a repo at PATH");
    std::println("\t--dry-run: show what actions would be taken");
}

void usageConfig() {
    std::println("confidant config [action]");
    std::println("View and modify configuration");
    std::println("\tdump: output the current configuration");
}

void usageLink() {
    std::println("confidant link [--dry-run,--verbose]");
    std::println("Create symlinks");
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
            usageInit();
            return 0;
        } else if (topic == "link") {
            usageLink();
            return 0;
        } else if (topic == "version") {
            std::println("{} version {}", argz, version);
            return 0;
        } else if (topic == "config") {
            usageConfig();
            return 0;
        } else {
            std::println(std::cerr, "{}: help topic '{}' is not known", argz, topic);
            return 1;
        }
    }
    
    if (argc == 2 && std::string(argv[1]) == "help") {
        usageGeneral();
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
        std::println("{} version {}", argz, version);
        return 0;
    } else if (args.got_subcommand(cmdConfig)) {
        if (cmdConfig->got_subcommand(cmdConfig_dump)) {
            confidant::configuration conf = confidant::config::serialize("confidant.ucl");
            confidant::debug::dumpConfig(conf);
            return 0;
        } else {
            usageConfig();
            return 1;
        }
    }

    return 0;

}