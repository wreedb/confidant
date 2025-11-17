# SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
#
# SPDX-License-Identifier: GPL-3.0-or-later

# confidant shell completions for fish

function __confidant_help_depth_1
    set -l cmd (commandline -opc)
    test (count $cmd) -eq 2
end

function __confidant_help_depth_2
    set -l cmd (commandline -opc)
    test (count $cmd) -eq 3
end

# global flags
complete -c confidant -s h -l help    -d "display help info"
complete -c confidant -s V -l version -d "display version info"
complete -c confidant -s u -l usage   -d "display usage info"
complete -c confidant -s v -l verbose -d "increase verbosity level"

# subcommands
complete -c confidant -f -n __fish_use_subcommand -a init      -d "initialize a confidant repository"
complete -c confidant -f -n __fish_use_subcommand -a config    -d "view configuration settings"
complete -c confidant -f -n __fish_use_subcommand -a link      -d "apply symlinks"
complete -c confidant -f -n __fish_use_subcommand -a usage     -d "display usage info"
complete -c confidant -f -n __fish_use_subcommand -a version   -d "display version info"
complete -c confidant -f -n __fish_use_subcommand -a help      -d "display help for subcommands"

# subcommand help
complete -c confidant -n "__fish_seen_subcommand_from help; and __confidant_help_depth_1" -f -a "init link config"
complete -c confidant -n "__fish_seen_subcommand_from help; and __fish_seen_subcommand_from config; and __confidant_help_depth_2" -f -a "dump get"

# subcommand: init
complete -c confidant -n "__fish_seen_subcommand_from init" -s d -l dry-run -d "simulate actions taken"
complete -c confidant -n "__fish_seen_subcommand_from init" -a "(__fish_complete_directories)"

# subcommand: config
complete -c confidant -n "__fish_seen_subcommand_from config" -f -a "dump get"
complete -c confidant -n "__fish_seen_subcommand_from config; and __fish_seen_subcommand_from dump" -s g -l global     -d "operate on global config"
complete -c confidant -n "__fish_seen_subcommand_from config; and __fish_seen_subcommand_from dump" -s f -l file    -r -d "specify a file path"
complete -c confidant -n "__fish_seen_subcommand_from config; and __fish_seen_subcommand_from get"  -s f -l file    -r -d "specify a file path"

# subcommand link
complete -c confidant -n "__fish_seen_subcommand_from link" -s d -l dry-run -d "simulate actions taken"
complete -c confidant -n "__fish_seen_subcommand_from link" -s f -l file    -d "specify a file path"