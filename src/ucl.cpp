// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <string_view>
#include <variant>
#include <format>
#include <print>
#include "confidant.hpp"
#include "util.hpp"
#include "ucl.hpp"
#include "xdg.hpp"
#include "logging.hpp"
#include "config.hpp"

namespace fs = std::filesystem;
namespace logger = confidant::logging;

using std::optional;

std::string cwd = std::string(fs::current_path());

namespace ucl {
    
    ucl_type_t Null = UCL_NULL;
    ucl_type_t Object = UCL_OBJECT;
    ucl_type_t Array = UCL_ARRAY;
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
    
    namespace get {
        optional<bool> boolean(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::Bool)
                return object.lookup(key).bool_value();
            else
                return std::nullopt;
        }
        
        optional<int> integer(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::Int)
                return object.lookup(key).int_value();
            else
                return std::nullopt;
        }
        
        optional<string> str(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::String)
                return object.lookup(key).string_value();
            else
                return std::nullopt;
        }
        
        optional<ucl::Ucl> list(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::Array)
                return object.lookup(key);
            else
                return std::nullopt;
        }
        
        optional<ucl::Ucl> node(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::Object)
                return object.lookup(key);
            else
                return std::nullopt;
        }
    };

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
    
    using ConfigValue = std::variant<
        bool,
        string,
        vector<string>,
        vector<confidant::config::link>,
        vector<confidant::config::templatelink>,
        confidant::config::link,
        confidant::config::templatelink
    >;
    
    optional<ConfigValue> get(const confidant::configuration& conf, string_view qry) {
        auto parts = util::split(qry);
        if (parts.empty()) return std::nullopt;
        if (parts.at(0) == "create-directories") {
            if (parts.size() == 1) return conf.create_dirs;
            return std::nullopt;
        }
        if (parts.at(0) == "repository") {
            if (parts.size() == 1) return conf.repo.url;
            if (parts.size() == 2 && parts.at(1) == "url") return conf.repo.url;
            return std::nullopt;
        }
        
        if (parts.at(0) == "links") {
            if (parts.size() == 1) {
                return conf.links;
            }
            if (parts.size() == 2) {
                for (const auto& link : conf.links) 
                    if (link.name == parts.at(1)) return link;
                return std::nullopt;
            }
            if (parts.size() == 3) {
                for (const auto& link : conf.links) {
                    if (link.name == parts.at(1)) {
                        if (parts.at(2) == "source") return link.source.string();
                        if (parts.at(2) == "dest") return link.destination.string();
                        if (parts.at(2) == "type")
                            return link.type == linkType::file ? "file" : "directory";
                    }
                }
            }
            return std::nullopt;
        }
        
        if (parts.at(0) == "templates") {
            if (parts.size() == 1) return conf.templates;
            if (parts.size() >= 2) {
                for (const auto& tmpl : conf.templates) {
                    if (tmpl.name == parts.at(1)) {
                        if (parts.size() == 2) return tmpl;
                        
                        if (parts.size() == 3) {
                            if (parts.at(2) == "source") return tmpl.source.string();
                            if (parts.at(2) == "dest") return tmpl.destination.string();
                            if (parts.at(2) == "items") return tmpl.items;
                        }
                    }
                }
            }
            return std::nullopt;
        }
        return std::nullopt;
    }
    std::string formatconfigvalue(const ConfigValue& value) {
        return std::visit([](auto&& arg) -> std::string {
            using T = std::decay_t<decltype(arg)>;
            
            if constexpr (std::is_same_v<T, bool>) {
                return arg ? "true" : "false";
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                return arg;
            }
            else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
                std::ostringstream oss;
                oss << "[\n";
                for (const auto& item : arg) {
                    oss << "  " << item << "\n";
                }
                oss << "]";
                return oss.str();
            }
            else if constexpr (std::is_same_v<T, std::vector<config::link>>) {
                std::ostringstream oss;
                for (const auto& link : arg) {
                    oss << link.name << ":\n";
                    oss << "  source: " << link.source.string() << "\n";
                    oss << "  dest: " << link.destination.string() << "\n";
                    oss << "  type: " << (link.type == config::linkType::file ? "file" : "directory") << "\n";
                }
                return oss.str();
            }
            else if constexpr (std::is_same_v<T, config::link>) {
                std::ostringstream oss;
                oss << "source: " << arg.source.string() << "\n";
                oss << "dest: " << arg.destination.string() << "\n";
                oss << "type: " << (arg.type == config::linkType::file ? "file" : "directory");
                return oss.str();
            }
            else if constexpr (std::is_same_v<T, std::vector<config::templatelink>>) {
                std::ostringstream oss;
                for (const auto& tmpl : arg) {
                    oss << tmpl.name << ":\n";
                    oss << "  source: " << tmpl.source.string() << "\n";
                    oss << "  dest: " << tmpl.destination.string() << "\n";
                    oss << "  items: [" << tmpl.items.size() << " items]\n";
                }
                return oss.str();
            }
            else if constexpr (std::is_same_v<T, config::templatelink>) {
                std::ostringstream oss;
                oss << "source: " << arg.source.string() << "\n";
                oss << "dest: " << arg.destination.string() << "\n";
                oss << "items:\n";
                for (const auto& item : arg.items) {
                    oss << "  - " << item << "\n";
                }
                return oss.str();
            }
            
            return "<unknown type>";
        }, value);
    }
    
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
        
        if (ucl::check(input["repository"], "url"))
            conf.repo.url = input["repository"]["url"].string_value();
        
    } else {
        conf.repo.url = "";
    }

    if (!ucl::check(input, "links")) {
        //logger::warn(PROJECT_NAME, "field {}links{} not specified", logger::color(1), logger::color(0));
    } else {

        ucl::Ucl links_ucl = input.lookup("links");
        int links_ucl_length = ucl::members(links_ucl);
        conf.links.reserve(links_ucl_length);

        for (const auto& object : links_ucl) {
            confidant::config::link link;
            // link name is ucl obj key
            link.name = object.key();
            
            if (ucl::check(object, "source") && object["source"].type() == ucl::String) {
                // source exists and is a string (correct)
                optional<string> s = ucl::get::str(object, "source");
                if (!s)
                    logger::fatal(PROJECT_NAME, 1,
                        "failed to get {} field {} as a string!",
                        logger::bolden(link.name),
                        logger::ital("source"));
                else
                    link.source = fs::path(s.value());

            } else if (ucl::check(object, "source") && object["source"].type() != ucl::String) {
                // source exists and is NOT a string (fatal)
                logger::fatal(PROJECT_NAME, 1, 
                    "link {} {} field must a string value!",
                    logger::bolden(link.name),
                    logger::ital("source"));

            } else {
                // source doesn't exist (fatal)
                logger::fatal(PROJECT_NAME, 1,
                    "link {} is missing a {} field!",
                    logger::bolden(link.name),
                    logger::ital("source"));
            }

            if (ucl::check(object, "dest")) {
                // normal dest path
                optional<string> tmp = ucl::get::str(object, "dest");
                if (!tmp) // nullopt
                    logger::fatal(PROJECT_NAME, 1,
                        "failed to get string value for {} destination",
                        logger::bolden(link.name));
                else
                    link.destination = fs::path(tmp.value());
            
            } else if (ucl::check(object, "destdir")) {
                // convert destdir + basename to destination path
                optional<string> s = ucl::get::str(object, "destdir");
                if (!s)
                    logger::fatal(PROJECT_NAME, 1,
                        "failed to get {} field {} as a string!",
                        logger::bolden(link.name),
                        logger::ital("destdir"));
                
                std::string destdir = object["destdir"].string_value();
                std::string bn = string(link.source.filename());
                link.destination = fs::path(std::format("{}/{}", destdir, bn));
            
            } else {
                // no dest or destdir (fatal)
                logger::fatal(PROJECT_NAME, 1,
                    "link {} is missing a {} or {} field!",
                    logger::bolden(link.name),
                    logger::ital("dest"),
                    logger::ital("destdir"));
            }

            if (!ucl::check(object, "type")) {
                // default value when it's not specified at all is 'file'
                link.type = confidant::config::linkType::file;
            
            } else {
                
                optional<string> t = ucl::get::str(object, "type");
                if (!t)
                    logger::fatal(PROJECT_NAME, 1,
                        "failed to get {} field {} as a string!",
                        logger::bolden(link.name),
                        logger::ital("type"));
                
                if (object["type"].string_value() == "file") {
                    link.type = confidant::config::linkType::file;
                
                } else if (object["type"].string_value() == "directory") {
                    link.type = confidant::config::linkType::directory;
                
                } else {
                    logger::warn(PROJECT_NAME,
                        "type {} is not recognized, expected one of: {} or {}, using default {}",
                        logger::ital(object["type"].string_value()),
                        logger::ital("file"),
                        logger::ital("directory"),
                        logger::bolden("file"));
                    link.type = confidant::config::linkType::file;
                }
            
            }
            conf.links.push_back(link);
        }
    }

    if (!ucl::check(input, "templates")) {
        logger::warnextra(PROJECT_NAME,
            "field {} not specified",
            logger::ital("templates"));
    } else {

        ucl::Ucl templates_ucl = input.lookup("templates");
        int numtemplates = ucl::members(templates_ucl);
        
        // reserve the amount ahead of time
        conf.templates.reserve(numtemplates);
        
        for (auto& tmpl : templates_ucl) {
            confidant::config::templatelink x;
            // templates name is the ucl obj key
            x.name = tmpl.key();
            
            if (ucl::check(tmpl, "source"))
                x.source = fs::path(tmpl["source"].string_value());
            else
                logger::fatal(PROJECT_NAME, 1,
                    "template {} is missing a {} value!",
                    logger::bolden(x.name),
                    logger::ital("source"));
                
            if (ucl::check(tmpl, "dest"))
                x.destination = fs::path(tmpl["dest"].string_value());
            else
                logger::fatal(PROJECT_NAME, 1,
                    "template {} is missing a {} value!",
                    logger::bolden(x.name),
                    logger::ital("dest"));
            
            if (ucl::check(tmpl, "items") && tmpl["items"].type() == ucl::Array) {
                // items field exists and is an array (normal, correct)
                int numitems = tmpl["items"].size();
                x.items.reserve(numitems);
                
                for (int i = 0; i < numitems; i++) {
                    optional<ucl::Ucl> obj = ucl::get::list(tmpl, "items");
                    if (!obj) {
                        logger::fatal(PROJECT_NAME, 1,
                            "failed to get {} items field as a list!",
                            logger::bolden(x.name));
                    } else {
                        x.items.push_back(obj.value().at(i).string_value());
                    }
                }
                
            } else if (ucl::check(tmpl, "items") && tmpl["items"].type() != ucl::Array) {
                // items field exists, but is not an array (fatal)
                logger::fatal(PROJECT_NAME, 1,
                    "template {} {} field is not a list!",
                    logger::bolden(x.name),
                    logger::ital("items"));
            } else {
                // items field doesn't exist (fatal)
                logger::fatal(PROJECT_NAME, 1,
                    "template {} has no {} list!",
                    logger::bolden(x.name),
                    logger::ital("items"));
            }
            conf.templates.push_back(x);
        }
    }
        
    return conf;
    
}
}; // END confidant::config
}; // END confidant