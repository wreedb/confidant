function __confidant_help_depth_1
    set -l cmd (commandline -opc)
    test (count $cmd) -eq 2
end

function __confidant_help_depth_2
    set -l cmd (commandline -opc)
    test (count $cmd) -eq 3
end

# usage
complete -c confidant -f -n "
    __fish_use_subcommand;
    and not __fish_seen_subcommand_from init;
    and not __fish_seen_subcommand_from help;
    and not __fish_seen_subcommand_from config;
    and not __fish_seen_subcommand_from usage;
    and not __fish_seen_subcommand_from version;
" -a usage -d "display brief usage info"

complete -c confidant -f -n "
    __fish_use_subcommand;
    and not __fish_seen_subcommand_from init;
    and not __fish_seen_subcommand_from help;
    and not __fish_seen_subcommand_from config;
    and not __fish_seen_subcommand_from usage;
    and not __fish_seen_subcommand_from version;
" -a version -d "display version info"


# -V --version
complete -c confidant -f -n "
    __fish_use_subcommand;
    and not __fish_seen_subcommand_from help;
" -s V -l version -d "display version info"



# -v --verbose
complete -c confidant -f -n "
    __fish_use_subcommand;
    and not __fish_seen_subcommand_from help;
    and not __fish_seen_subcommand_from usage;
    and not __fish_seen_subcommand_from version;
" -s v -l verbose -d "increase verbosity"

# -h --help
complete -c confidant -f -n "
    __fish_use_subcommand;
    and not __fish_seen_subcommand_from help;
    and not __fish_seen_subcommand_from version;
    and not __fish_seen_subcommand_from usage;
" -s h -l help -d "display help info"

# help
complete -c confidant -f -n "
    __fish_use_subcommand;
    and not __fish_seen_subcommand_from init;
    and not __fish_seen_subcommand_from help;
    and not __fish_seen_subcommand_from config;
    and not __fish_seen_subcommand_from usage;
    and not __fish_seen_subcommand_from version;
" -a help -d "display help for subcommands"
# help <action>
complete -c confidant -n "__fish_seen_subcommand_from help; and __confidant_help_depth_1" -f -a "init link config"
complete -c confidant -n "__fish_seen_subcommand_from help; and __fish_seen_subcommand_from config; and __confidant_help_depth_2" -f -a "dump get"

complete -c confidant -f -n "
    __fish_use_subcommand;
    and not __fish_seen_subcommand_from init;
    and not __fish_seen_subcommand_from help;
    and not __fish_seen_subcommand_from config;
    and not __fish_seen_subcommand_from usage;
    and not __fish_seen_subcommand_from version;
" -a config -d "view configuration"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and not __fish_seen_subcommand_from dump;
    and not __fish_seen_subcommand_from get
" -f -a dump -d "display current configuration"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and not __fish_seen_subcommand_from dump;
    and not __fish_seen_subcommand_from get
" -f -a dump -d "display current configuration"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and not __fish_seen_subcommand_from dump;
    and not __fish_seen_subcommand_from get
" -f -s h -l help -d "display help info"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and __fish_seen_subcommand_from get;
    and not __fish_seen_subcommand_from dump;
" -f -s h -l help -d "display help info"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and __fish_seen_subcommand_from dump;
    and not __fish_seen_subcommand_from get;
" -f -s h -l help -d "display help info"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and not __fish_seen_subcommand_from dump;
    and not __fish_seen_subcommand_from get
" -f -a get -d "search for a value by name"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and __fish_seen_subcommand_from get
" -f -s h -l help -d "display help info"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and __fish_seen_subcommand_from dump
" -f -s f -l file -d "specify a file path"

complete -c confidant -n "
    __fish_seen_subcommand_from config;
    and __fish_seen_subcommand_from dump;
" -s g -l global -d "display global configuration"

complete -c confidant -n __fish_use_subcommand -a init -d "initialize a repository"
complete -c confidant -n "__fish_seen_subcommand_from init" -s h -l help -d "display help info"
complete -c confidant -n __fish_use_subcommand -a link -d "apply symlinks"
complete -c confidant -n "__fish_seen_subcommand_from link" -s h -l help -d "display help info"

complete -c confidant -n "__fish_seen_subcommand_from link" -s t -l tags -d "specify tagged entries to apply"
complete -c confidant -n "__fish_seen_subcommand_from link" -s f -l file -d "specify a file path"
complete -c confidant -n "__fish_seen_subcommand_from link" -s d -l dry-run -d "simulate actions only"