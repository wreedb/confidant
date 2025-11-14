// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <sstream>
#include <stdexcept>
#include "confidant.hpp"
#include "util.hpp"
#include "ucl.hpp"
#include "xdg.hpp"
#include "logging.hpp"
#include "config.hpp"

namespace fs = std::filesystem;
namespace logger = confidant::logging;

std::string cwd = std::string(fs::current_path());

namespace ucl {
    ucl_type_t Null = UCL_NULL;
    ucl_type_t Object = UCL_OBJECT;
    ucl_type_t Int = UCL_INT;
    ucl_type_t Float = UCL_FLOAT;
    ucl_type_t String = UCL_STRING;
    ucl_type_t Bool = UCL_BOOLEAN;
    ucl_type_t Time = UCL_TIME;
    ucl_type_t UserData = UCL_USERDATA;

    namespace emit {
        ucl_emitter json = UCL_EMIT_JSON;
        ucl_emitter jsonc = UCL_EMIT_JSON_COMPACT;
        ucl_emitter yaml = UCL_EMIT_YAML;
        ucl_emitter yml = UCL_EMIT_YAML;
        ucl_emitter msgpack = UCL_EMIT_MSGPACK;
        ucl_emitter config = UCL_EMIT_CONFIG;
        ucl_emitter max = UCL_EMIT_MAX;
    }; // END ucl::emit

    namespace flags {
        ucl_parser_flags_t Default = UCL_PARSER_DEFAULT;
        ucl_parser_flags_t LowercaseKeys = UCL_PARSER_KEY_LOWERCASE;
        ucl_parser_flags_t ZeroCopy = UCL_PARSER_ZEROCOPY;
        ucl_parser_flags_t NoTime = UCL_PARSER_NO_TIME;
        ucl_parser_flags_t ExplicitArrays = UCL_PARSER_NO_IMPLICIT_ARRAYS;
        ucl_parser_flags_t SaveComments = UCL_PARSER_SAVE_COMMENTS;
        ucl_parser_flags_t NoMacros = UCL_PARSER_DISABLE_MACRO;
        ucl_parser_flags_t NoVars = UCL_PARSER_NO_FILEVARS;
    }; // END ucl::flags

    namespace var {

        std::map<std::string, std::string> add(const std::string& name, const std::string& value, std::map<std::string, std::string>& vm) {
            if (vm.contains(name)) {
                if (vm.at(name) == value)
                    return vm;
                else
                    vm.insert_or_assign(name, value);
            } else {
                vm.insert(std::pair<std::string, std::string>{name, value});
            }
            return vm;
        }

        std::map<std::string, std::string> fromenv(const std::string& key, std::map<std::string, std::string>& vm, const std::string& fallback) {
            std::optional<std::string> env = util::getenv(key);
            if (!env) {
                vm = ucl::var::add(key, env.value(), vm);
            } else {
                vm = ucl::var::add(key, fallback, vm);
            }
            return vm;
        }
    
    }; // END ucl::var

    bool check(const ucl::Ucl& obj, const std::string& key) {
        if (obj.lookup(key).type() == UCL_NULL)
            return false;
        else
            return true;
    }

    int members(const ucl::Ucl& object) {
        if (object.type() != UCL_OBJECT) return 0;
        int n = 0;
        for ([[maybe_unused]] const auto& _ : object) n++;
        return n;
    }

}; // END ucl

namespace confidant {

std::string substitute(const std::string& tmpl, const std::string& item) {
    const std::string placeholder = "%{item}";
    std::string result = tmpl;
    
    size_t pos = 0;
    while ((pos = result.find(placeholder, pos)) != std::string::npos) {
        result.replace(pos, placeholder.length(), item);
        pos += item.length();
    }
    return result;
}

namespace config {

namespace global {

confidant::settings serialize(std::string_view path) {
    confidant::settings conf;
    
    if (!fs::exists(path))
        logger::fatal(PROJECT_NAME, 1, "configuration file {} does not exist!", util::unexpandhome(path));
    
    std::ifstream handle((fs::path(path)));
    
    if (!handle.is_open())
        logger::fatal(PROJECT_NAME, 1, "failed to read from {}", util::unexpandhome(path));
    
    std::stringstream buffer;
    buffer << handle.rdbuf();
    handle.close();
    
    // holds parsing errors
    std::string errorstring;
    ucl::Ucl object = ucl::Ucl::parse(buffer.str(), errorstring, UCL_DUPLICATE_APPEND);
    
    // libucl encountered an error while parsing
    if (!errorstring.empty()) {
        logger::error(PROJECT_NAME, "failed to parse configuration at {}", util::unexpandhome(path));
        std::println(std::cerr, "parser error: {}", errorstring);
        std::exit(1);
    }
    
    if (!ucl::check(object, "create-directories"))
        logger::warn(PROJECT_NAME, "no setting provided for 'create-directories', using default: true");
        
    else if (object["create-directories"].type() != ucl::Bool)
        logger::error(PROJECT_NAME, "invalid value assigned to 'create-directories'\nexpected one of: true, false, yes, no, on, off\nusing default: true");
        
    else
        conf.create_dirs = object["create-directories"].bool_value();
    
    if (!ucl::check(object, "log-level")) {
        logger::warn(PROJECT_NAME, "no setting provided for 'log-level', using default: normal");  
    } else if (object["log-level"].type() == ucl::String) {
        std::string val = object["log-level"].string_value();
        if        (val == "quiet") {
            conf.loglevel = ansi::verbosity::quiet;
        } else if (val == "normal") {
            conf.loglevel = ansi::verbosity::normal;
        } else if (val == "info") {
            conf.loglevel = ansi::verbosity::info;
        } else if (val == "debug") {
            conf.loglevel = ansi::verbosity::debug;
        } else if (val == "trace") {
            conf.loglevel = ansi::verbosity::trace;
        } else {
            logger::error(PROJECT_NAME, "log-level setting '{}' is not valid\nexpected one of: quiet, normal (default), info, debug, trace (or 0, 1, 2, 3, 4); using default", val);           
        }
    } else if (object["log-level"].type() == ucl::Int) {
        int val = object["log-level"].int_value();
        switch (val) {
            case ansi::verbosity::quiet:
                conf.loglevel = ansi::verbosity::quiet;
                break;
            case ansi::verbosity::normal:
                conf.loglevel = ansi::verbosity::normal;
                break;
            case ansi::verbosity::info:
                conf.loglevel = ansi::verbosity::info;
                break;
            case ansi::verbosity::debug:
                conf.loglevel = ansi::verbosity::debug;
                break;
            case ansi::verbosity::trace:
                conf.loglevel = ansi::verbosity::trace;
            default:
                logger::error(PROJECT_NAME, "log-level setting '{}' is not valid\nexpected one of: quiet, normal (default), info, debug, trace (or 0, 1, 2, 3, 4); using default", val);           
                break;
        }
    }
    
    return conf;
}
    
}; // END confidant::config::global

confidant::configuration serialize(std::string_view path, const confidant::settings& globals) {
    // ensure the file exists
    if (!fs::exists(fs::path(path))) {
        std::println(std::cerr, "error: path '{}' does not exist", path);
        std::exit(1);
    }
    
    // get a file stream, ensure it properly opened
    std::ifstream handle((fs::path(path)));
    if (!handle.is_open()) {
        println(cerr, "error: failed opening file '{}'", path);
        std::exit(1);
    }

    // read the file into a buffer, close the file stream
    std::stringstream buffer;
    buffer << handle.rdbuf();
    handle.close();

    // holds any errors in parsing
    std::string uclerror;
    std::string fileContents = buffer.str();

    // built-in variables for the config file
    std::map<std::string, std::string> vars;

    // get some xdg variables and add them to vars
    std::map<std::string, fs::path> xdghomes = xdg::homes();
    vars = ucl::var::add("XDG_CONFIG_HOME", xdghomes["XDG_CONFIG_HOME"].string(), vars);
    vars = ucl::var::add("XDG_CACHE_HOME",  xdghomes["XDG_CACHE_HOME"].string(),  vars);
    vars = ucl::var::add("XDG_DATA_HOME",   xdghomes["XDG_DATA_HOME"].string(),   vars);
    vars = ucl::var::add("XDG_STATE_HOME",  xdghomes["XDG_STATE_HOME"].string(),  vars);

    std::optional<std::string> homedir = util::getenv("HOME");
    if (!homedir)
        throw std::runtime_error("HOME is not set in the environment!");
    
    vars = ucl::var::add("HOME", homedir.value(), vars);
    vars = ucl::var::add("REPO", cwd, vars);

    confidant::configuration conf;
    confidant::config::repository repository;
    std::vector<confidant::config::link> links;
    confidant::config::linkFrom linkFrom;
    
    ucl::Ucl input = ucl::Ucl::parse(fileContents, vars, uclerror, UCL_DUPLICATE_APPEND);

    if (!uclerror.empty()) {
        println(std::cerr, "parsing error: {}", uclerror);
        std::exit(1);
    }
    
    // BEGIN serializing
    
    // override global setting for this repository
    if (ucl::check(input, "create-directories")) {
        if (input["create-directories"].type() == ucl::Bool) {
            conf.create_dirs = input["create-directories"].bool_value();
        } else {
            logger::error(
                    PROJECT_NAME,
                    "create-directories value is not valid,\nexpected one of: true (default), false, yes, no, on, off; using global setting: {}",
                    globals.create_dirs);
            conf.create_dirs = globals.create_dirs;
        }
    }

    if (ucl::check(input, "repository")) {
        if (ucl::check(input["repository"], "url")) {
            repository.url = input["repository"]["url"].string_value();
        }
    } else {
        repository.url = "";
    }

    if (!ucl::check(input, "link")) {
        logger::warn(PROJECT_NAME, "field 'link' not specified");
    } else {

        ucl::Ucl links_ucl = input.lookup("link");
        int links_ucl_length = ucl::members(links_ucl);
        links.reserve(links_ucl_length);

        for (const auto& object : links_ucl) {
            confidant::config::link link;
            link.name = object.key();
            link.source = fs::path(object["source"].string_value());

            if (ucl::check(object, "dest")) {
                link.destination = fs::path(object["dest"].string_value());
            } else if (ucl::check(object, "destdir")) {
                std::string destdir = object["destdir"].string_value();
                std::string bn = string(link.source.filename());
                link.destination = fs::path(std::format("{}/{}", destdir, bn));
            }

            if (!ucl::check(object, "type")) {
                link.type = confidant::config::linkType::file;
            
            } else {
            
                if (object["type"].string_value() == "file") {
                    link.type = confidant::config::linkType::file;
                
                } else if (object["type"].string_value() == "directory") {
                    link.type = confidant::config::linkType::directory;
                
                } else {
                    std::println(std::cerr, "warning: type '{}' is not recognized, expected one of 'file' or 'directory'. Using default value of 'file'.", object["type"].string_value());
                    link.type = confidant::config::linkType::file;
                }
            
            }
            links.push_back(link);
        }
    }

    if (!ucl::check(input, "link-from")) {
        logger::warn(PROJECT_NAME, "field 'link-from' not specified");
    } else {

        ucl::Ucl linkfrom_ucl = input.lookup("link-from");
    
        linkFrom.from = fs::path(linkfrom_ucl["from"].string_value());
        linkFrom.to   = fs::path(linkfrom_ucl["to"].string_value());

        auto linkfrom_items = linkfrom_ucl.lookup("items");
        int numitems = linkfrom_items.size();
    
        std::vector<std::string> items;
        items.reserve(numitems);

        for (int n = 0; n < numitems; n++) {
            items.push_back(linkfrom_items.at(n).string_value());
        }
        linkFrom.items = items;
    }

    conf.repo = repository;
    conf.links = links;
    conf.linkFrom = linkFrom;

    return conf;

}
}; // END confidant::config
}; // END confidant