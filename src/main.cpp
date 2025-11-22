// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

// cli
#include <lyra/lyra.hpp>
#include <lyra/group.hpp>
#include <lyra/parser.hpp>
#include <lyra/help.hpp>

// std
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
        std::string file = std::format("{}/confidant.ucl", path);
        bool self = false;
        bool help = false;
        bool dry = false;
    };  
};

namespace opts {
    namespace cmd {
        namespace link {
            lyra::command self = lyra::command("link", [](const lyra::group&) { args::link::self = true; }).help("apply symlinks");
            lyra::help help = lyra::help(args::link::help);
            lyra::opt dry = lyra::opt(args::link::dry)["-d"]["--dry-run"]("simulate actions only");
            lyra::opt tags = lyra::opt(args::link::tags, "tags")["-t"]["--tags"]("apply tagged links");
            lyra::opt file = lyra::opt(args::link::file, "path")["-f"]["--file"]("specify a file path to read");
        };
        namespace help {
            lyra::command self = lyra::command("help", [](const lyra::group&) { args::help::self = true; }).help("display help for subcommands");
            namespace config {
                lyra::command self = lyra::command("config", [](const lyra::group&) { args::config::help = true; });
                lyra::command dump = lyra::command("dump", [](const lyra::group&) { args::config::dump::help = true; });
                lyra::command get  = lyra::command("get", [](const lyra::group&) { args::config::get::help = true; });
            };
            lyra::command init = lyra::command("init", [](const lyra::group&) { args::init::help = true; });
            lyra::command link = lyra::command("link", [](const lyra::group&) { args::link::help = true; });
        };
        lyra::command usage = lyra::command("usage", [](const lyra::group&) { args::usage = true; }).help("display brief usage info");
        lyra::command version = lyra::command("version", [](const lyra::group&) { args::version = true; }).help("display version info");
        namespace init {
            lyra::command self = lyra::command("init", [](const lyra::group&) { args::init::self = true; }).help("initialize a repository");
            lyra::help help = lyra::help(args::init::help);
            lyra::arg path = lyra::arg(args::init::path, "path")("the directory path to initialize");
            lyra::opt dry = lyra::opt(args::init::dry)["-d"]["--dry-run"]("simulate actions only");
        };
        namespace config {
            lyra::command self = lyra::command("config", [](const lyra::group&) { args::config::self = true; }).help("introspect configuration settings");
            lyra::opt help = lyra::opt(args::config::help).name("-h").name("--help").name("-?").optional();
            namespace dump {
                lyra::command self = lyra::command("dump", [](const lyra::group&) { args::config::dump::self = true; }).help("display current configuration settings").optional();
                lyra::help help = lyra::help(args::config::dump::help);
                lyra::opt global = lyra::opt(args::config::dump::global)["-g"]["--global"]("display global configuration").optional();
                lyra::opt json = lyra::opt(args::config::dump::json)["-j"]["--json"]("display configuration in JSON format").optional();
                lyra::opt file = lyra::opt(args::config::dump::file, "path")["-f"]["--file"]("specify a file path");
            };
            namespace get {
                lyra::command self = lyra::command("get", [](const lyra::group&) { args::config::get::self = true; }).help("find configuration value by name").optional();
                lyra::arg query = lyra::arg(args::config::get::query, "query").help("setting to search for");
                lyra::help help = lyra::help(args::config::get::help);
                lyra::opt global = lyra::opt(args::config::get::global)["-g"]["--global"]("query from global configuration");
                lyra::opt file = lyra::opt(args::config::get::file, "path")["-f"]["--file"]("specify a file path");
            };
        };
    };

}; 

namespace flags {
    lyra::help help = lyra::help(args::help::self).description("display help information");
    lyra::opt usage = lyra::opt(args::usage)["-u"]["--usage"]("display brief usage info");
    lyra::opt version = lyra::opt(args::version)["-V"]["--version"]("display version info");
    lyra::opt verbose = lyra::opt(args::verbose)["-v"]["--verbose"]("increase verbosity");
    lyra::opt quiet = lyra::opt(args::quiet)["-q"]["--quiet"]("supress non-error messages");
};

int main(int argc, const char *argv[]) {
    
    std::string version = PROJECT_VERSION;
    std::string argz = util::stripargz(argv[0]);
    
    gconfig::settings gconf = gconfig::serialize(help::defaults::global_config_path());
    
    opts::cmd::link::self |= flags::quiet;
    opts::cmd::link::self |= flags::verbose;
    
    opts::cmd::init::self |= flags::quiet;
    opts::cmd::init::self |= flags::verbose;
    
    opts::cmd::config::self |= flags::quiet;
    opts::cmd::config::self |= flags::verbose;
    
    opts::cmd::config::get::self |= flags::quiet;
    opts::cmd::config::get::self |= flags::verbose;
    
    opts::cmd::config::dump::self |= flags::quiet;
    opts::cmd::config::dump::self |= flags::verbose;
    
    opts::cmd::link::self
        |= opts::cmd::link::dry;
    
    opts::cmd::link::self
        |= opts::cmd::link::tags;
    
    opts::cmd::link::self
        |= opts::cmd::link::file;
    
    opts::cmd::link::self
        |= opts::cmd::link::help;
    
    opts::cmd::help::self
        |= opts::cmd::help::link;
    
    opts::cmd::help::config::self
        |= opts::cmd::help::config::dump;
    
    opts::cmd::help::config::self
        |= opts::cmd::help::config::get;
    
    opts::cmd::help::self
        |= opts::cmd::help::config::self;
    
    opts::cmd::help::self
        |= opts::cmd::help::init;
    
    opts::cmd::config::self
        |= opts::cmd::config::help;
    
    opts::cmd::config::dump::self
        |= opts::cmd::config::dump::help;
    
    opts::cmd::config::dump::self
        |= opts::cmd::config::dump::global;
    
    opts::cmd::config::dump::self
        |= opts::cmd::config::dump::json;
    
    opts::cmd::config::dump::self
        |= opts::cmd::config::dump::file;
            
    opts::cmd::config::get::self
        |= opts::cmd::config::get::help;
    
    opts::cmd::config::get::self
        |= opts::cmd::config::get::global;
    
    opts::cmd::config::get::self
        |= opts::cmd::config::get::file;
    
    opts::cmd::config::get::self
        |= opts::cmd::config::get::query;
    
    opts::cmd::config::self
        |= opts::cmd::config::dump::self;
    
    opts::cmd::config::self
        |= opts::cmd::config::get::self;
    
    opts::cmd::init::self
        |= opts::cmd::init::dry;
    
    opts::cmd::init::self
        |= opts::cmd::init::path;
    
    opts::cmd::init::self
        |= opts::cmd::init::help;
    
    lyra::cli cli;
    cli |= opts::cmd::link::self;
    cli |= opts::cmd::config::self;
    cli |= opts::cmd::init::self;
    cli |= opts::cmd::help::self;
    cli |= opts::cmd::usage;
    cli |= opts::cmd::version;
    cli |= flags::help;
    cli |= flags::usage;
    cli |= flags::version;
    cli |= flags::quiet;
    cli |= flags::verbose;
    
    lyra::parse_result res = cli.parse ({ argc, argv });
    
    if (!usecolorp) {
        // environment NO_COLOR presence always honored
        options::global::color = false;
        gconfig::color = false;
    } else {
        options::global::color = gconf.color;
        gconfig::color = gconf.color;
    }
    
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
            msg::pretty("wrote sample config file to {}", args::init::file);
            return 0;
        } else {
            msg::pretty("wrote sample config file to {}", args::init::file);
            help::defaults::write_local_config(args::init::path);
            return 0;
        }
    }
    
    help::general::usage(argz);
    return 1;
}