// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

// cli
#include <lyra/lyra.hpp>
#include <lyra/group.hpp>
#include <lyra/parser.hpp>
#include <lyra/help.hpp>

// std
#include <format>
#include <print>
#include <filesystem>
#include <string>
#include <string_view>

// project-local
#include "settings/local.hpp"
#include "settings/global.hpp"
#include "util.hpp"
#include "help.hpp"
#include "options.hpp"
#include "fmt.hpp"
#include "msg.hpp"
#include "actions/dump.hpp"
#include "actions/link.hpp"
#include "actions/get.hpp"

// meson
#include "config.hpp"

// detect: NO_COLOR env var, output is not a terminal, etc.
const static bool usecolorp = util::usecolorp();

namespace gconfig = confidant::config::global;
namespace lconfig = confidant::config::local;

namespace actions = confidant::actions;

namespace fs = std::filesystem;

namespace args {
    
    namespace link {
        
        bool self = false;
        bool help = false;
        bool dry = false;
        std::string tags;
        std::string file = fs::current_path().string() + "/confidant.ucl";
    
    };
    
    namespace config {
        bool self = false;
        bool help = false;
        
        namespace dump {
            bool self = false;
            bool help = false;
            bool json = false;
            bool global = false;
            std::string file = fs::current_path().string() + "/confidant.ucl";
        };
        
        namespace get {
            bool self = false;
            bool help = false;
            bool global = false;
            std::string query;
            std::string file = fs::current_path().string() + "/confidant.ucl";
        };
    };
    
    bool usage = false;
    bool version = false;
    bool verbose = false;
    bool quiet = false;
    
    namespace help {
        bool self = false;
        namespace config {
            bool self = false;
            bool dump = false;
            bool get = false;
        };
        bool init = false;
        bool link = false;
    };
    
    namespace init {
        std::string path = fs::current_path().string();
        std::string file = std::format("{}/confidant.ucl", args::init::path);
        bool self = false;
        bool help = false;
        bool dry = false;
    };  
};

namespace cmd {
    namespace link {
        lyra::command self = lyra::command("link", [](const lyra::group&) { args::link::self = true; });
        lyra::help help = lyra::help(args::link::help);
        lyra::opt dry = lyra::opt(args::link::dry)["-d"]["--dry-run"];
        lyra::opt tags = lyra::opt(args::link::tags, "tags")["-t"]["--tags"];
        lyra::opt file = lyra::opt(args::link::file, "path")["-f"]["--file"];
    };
    namespace help {
        lyra::command self = lyra::command("help", [](const lyra::group&) { args::help::self = true; });
        namespace config {
            lyra::command self = lyra::command("config", [](const lyra::group&) { args::config::help = true; });
            lyra::command dump = lyra::command("dump", [](const lyra::group&) { args::config::dump::help = true; });
            lyra::command get  = lyra::command("get", [](const lyra::group&) { args::config::get::help = true; });
        };
        lyra::command init = lyra::command("init", [](const lyra::group&) { args::init::help = true; });
        lyra::command link = lyra::command("link", [](const lyra::group&) { args::link::help = true; });
    };
    lyra::command usage = lyra::command("usage", [](const lyra::group&) { args::usage = true; });
    lyra::command version = lyra::command("version", [](const lyra::group&) { args::version = true; });
    namespace init {
        lyra::command self = lyra::command("init", [](const lyra::group&) { args::init::self = true; });
        lyra::help help = lyra::help(args::init::help);
        lyra::arg path = lyra::arg(args::init::path, "path");
        lyra::opt dry = lyra::opt(args::init::dry)["-d"]["--dry-run"];
    };
    namespace config {
        lyra::command self = lyra::command("config", [](const lyra::group&) { args::config::self = true; });
        lyra::opt help = lyra::opt(args::config::help).name("-h").name("--help").name("-?").optional();
        namespace dump {
            lyra::command self = lyra::command("dump", [](const lyra::group&) { args::config::dump::self = true; }).optional();
            lyra::help help = lyra::help(args::config::dump::help);
            lyra::opt global = lyra::opt(args::config::dump::global)["-g"]["--global"].optional();
            lyra::opt json = lyra::opt(args::config::dump::json)["-j"]["--json"].optional();
            lyra::opt file = lyra::opt(args::config::dump::file, "path")["-f"]["--file"];
        };
        namespace get {
            lyra::command self = lyra::command("get", [](const lyra::group&) { args::config::get::self = true; }).optional();
            lyra::arg query = lyra::arg(args::config::get::query, "query");
            lyra::help help = lyra::help(args::config::get::help);
            lyra::opt global = lyra::opt(args::config::get::global)["-g"]["--global"];
            lyra::opt file = lyra::opt(args::config::get::file, "path")["-f"]["--file"];
        };
    };
};

namespace flags {
    lyra::help help = lyra::help(args::help::self);
    lyra::opt usage = lyra::opt(args::usage)["-u"]["--usage"];
    lyra::opt version = lyra::opt(args::version)["-V"]["--version"];
    lyra::opt verbose = lyra::opt(args::verbose)["-v"]["--verbose"];
    lyra::opt quiet = lyra::opt(args::quiet)["-q"]["--quiet"];
};

int main(int argc, const char *argv[]) {
    
    std::string version = PROJECT_VERSION;
    std::string argz = util::stripargz(argv[0]);
    
    gconfig::color = usecolorp;
    options::global::color = usecolorp;
    
    fs::path gconfpath = help::defaults::global_config_path();
    // check for global config file; try to create it
    if (!fs::exists(gconfpath)) {
        try {
            if (!fs::exists(gconfpath.parent_path()))
                try {
                    std::filesystem::create_directory(gconfpath.parent_path());
                } catch (const fs::filesystem_error& err) {
                    msg::error("failed to create parent directory {} for global config file!", gconfpath.parent_path().string());
                    std::cout << err.what() << std::endl;
                }
            // write the default global config
            help::defaults::write_global_config(gconfpath.string());
        } catch (const fs::filesystem_error& e) {
            msg::error("failed to create global config file at {}!", gconfpath.string());
            std::cout << e.what() << std::endl;
        }
    }
    
    // found the global config, serialize it
    gconfig::settings gconf = gconfig::serialize(gconfpath.string());
    
    // prepare cli
    lyra::cli cli;
    cli
    // help subcommand
    .add_argument(cmd::help::self
        .add_argument(cmd::help::init)
        .add_argument(cmd::help::link)
        .add_argument(cmd::help::config::self
            .add_argument(cmd::help::config::dump)
            .add_argument(cmd::help::config::get)))
    // init subcommand
    .add_argument(cmd::init::self
        .add_argument(cmd::init::help)
        .add_argument(cmd::init::dry)
        .add_argument(cmd::init::path)
        .add_argument(flags::verbose)
        .add_argument(flags::quiet))
    // link subcommand
    .add_argument(cmd::link::self
        .add_argument(cmd::link::dry)
        .add_argument(cmd::link::file)
        .add_argument(cmd::link::tags)
        .add_argument(cmd::link::help)
        .add_argument(flags::verbose)
        .add_argument(flags::quiet))
    // config subcommand
    .add_argument(cmd::config::self
        // config get subcommand
        .add_argument(cmd::config::get::self
            .add_argument(cmd::config::get::help)
            .add_argument(cmd::config::get::file)
            .add_argument(cmd::config::get::global)
            .add_argument(cmd::config::get::query)
            .add_argument(flags::quiet)
            .add_argument(flags::verbose))
        // config dump subcommand
        .add_argument(cmd::config::dump::self
            .add_argument(cmd::config::dump::file)
            .add_argument(cmd::config::dump::help)
            .add_argument(cmd::config::dump::global)
            .add_argument(cmd::config::dump::json)
            .add_argument(flags::quiet)
            .add_argument(flags::verbose))
        .add_argument(flags::verbose)
        .add_argument(flags::quiet)
        .add_argument(cmd::config::help))
    .add_argument(flags::usage)
    .add_argument(flags::quiet)
    .add_argument(flags::verbose)
    .add_argument(flags::version)
    .add_argument(flags::help);
    
    // parse the command line
    lyra::parse_result res = cli.parse ( { argc, argv } );
    
    /* apply global configurations */
    
    // color
    if (!usecolorp) {
        // environment NO_COLOR presence always honored
        options::global::color = false;
        gconfig::color = false;
    } else {
        options::global::color = gconf.color;
        gconfig::color = gconf.color;
    }
    
    // log-level
    if (args::quiet && args::verbose) {
        msg::error("--quiet and --verbose are mutually exclusive.");
        return 1;
    } else if (args::quiet && !args::verbose) {    
        gconfig::loglevel = util::verbose::quiet;
    } else if (args::verbose && !args::quiet) {
        gconfig::loglevel = util::verbose::trace;
    } else {
        // only override the config setting when options are passed
        gconfig::loglevel = gconf.loglevel;
    }
    
    // create-directories
    gconfig::createdirs = gconf.createdirs;
    
    if (!res) {
        msg::error("unable to parse command line");
        std::print("{}: [", fmt::fg::red("arguments"));
        for (int n = 1; n < argc; n++) {
            std::print("{}{}", argv[n],
                (n == (argc - 1) ? "" : " "));
        }
        std::println("]\n");
        help::general::usage(argz);
        return 1;
    }
    
    if (args::config::get::query.starts_with('-')) {
        args::config::get::query = "";
        args::config::get::help = true;
    }
    
    if (args::init::path.starts_with('-') || args::init::path == "-d" || args::init::path == "--dry-run") {
        args::init::path = "";
        args::init::help = true;
    }
    
    if (args::help::self) {
        if (args::init::help) help::init::help(argz);
        else if (args::link::help) help::link::help(argz);
        else if (args::config::help) {
            if (args::config::dump::help) help::config::dump::help(argz);
            else if (args::config::get::help) help::config::get::help(argz);
            else help::config::help(argz);
        }
        else help::general::help(argz);
        return 0;
    }
    
    if (args::usage) {
        help::general::usage(argz);
        return 0;
    }
    
    if (args::version) {
        msg::pretty("version {}", version);
        return 0;
    }
    
    if (args::config::help) {
        help::config::help(argz);
        return 0;
    }
    
    if (args::config::get::help) {
        help::config::get::help(argz);
        return 0;
    }
    
    if (args::config::dump::help) {
        help::config::dump::help(argz);
        return 0;    
    }
    
    if (args::init::help) {
        help::init::help(argz);
        return 0;
    }
    
    if (args::link::help) {
        help::link::help(argz);
        return 0;
    }
    
    if (args::config::self) {
        
        if (args::config::dump::self) {
            
            if (args::config::dump::global) {
                if (args::config::dump::json) {
                    actions::dump::json::global(help::defaults::global_config_path());
                    return 0;
                } else {
                    actions::dump::global(gconf);
                    return 0;
                }
            } else {
                if (args::config::dump::json) {
                    actions::dump::json::local(args::config::dump::file);
                    return 0;
                } else {
                    lconfig::settings lconf = lconfig::serialize(args::config::dump::file, gconf);
                    actions::dump::local(lconf);
                    return 0;
                }
            }
            
        } else if (args::config::get::self) {
            if (args::config::get::global) {
                auto res = actions::get::global(gconf, args::config::get::query);
                if (!res) {
                    msg::error("setting {} not found in configuration", fmt::bolden(args::config::get::query));
                    return 1;
                }
                std::println("{}", actions::get::formatglobalvalue(res.value()));
                return 0;
            } else {
                lconfig::settings lconf = lconfig::serialize(args::config::dump::file, gconf);
                auto res = actions::get::local(lconf, args::config::get::query);
                if (!res) {
                    msg::error("setting {} not found in configuration", fmt::bolden(args::config::get::query));
                    return 1;
                }
                std::println("{}", actions::get::formatlocalvalue(res.value()));
                return 0;
            }
        }
    }
    
    if (args::link::self) {
        std::vector<std::string_view> tags;
        
        if (!args::link::tags.empty())
            tags = util::splittags(args::link::tags);
        
        lconfig::settings lconf = lconfig::serialize(args::link::file, gconf);
        int n = actions::link::linknormal(lconf, gconf, tags, args::link::dry);
        if (n != 0) return n;
        int t = actions::link::linktemplate(lconf, gconf, tags, args::link::dry);
        if (t != 0) return n;
        return n + t;
    }
    
    if (args::init::self) {
        if (args::init::dry) {
            msg::pretty("wrote sample config file to {}", fs::relative(args::init::path).string() + "/confidant.ucl");
            return 0;
        } else {
            msg::pretty("wrote sample config file to {}", fs::relative(args::init::path).string() + "/confidant.ucl");
            help::defaults::write_local_config(args::init::path);
            return 0;
        }
    }
    // nothing else was properly parsed or caught to cause process to stop;
    // show usage and exit 1.
    help::general::usage(argz);
    return 1;
}