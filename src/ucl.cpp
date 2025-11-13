// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <stdexcept>
#include "util.hpp"
#include "ucl.hpp"
#include "xdg.hpp"

namespace fs = std::filesystem;

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
}; // NOTE: end namespace ucl::emit

namespace flags {
ucl_parser_flags_t Default = UCL_PARSER_DEFAULT;
ucl_parser_flags_t LowercaseKeys = UCL_PARSER_KEY_LOWERCASE;
ucl_parser_flags_t ZeroCopy = UCL_PARSER_ZEROCOPY;
ucl_parser_flags_t NoTime = UCL_PARSER_NO_TIME;
ucl_parser_flags_t ExplicitArrays = UCL_PARSER_NO_IMPLICIT_ARRAYS;
ucl_parser_flags_t SaveComments = UCL_PARSER_SAVE_COMMENTS;
ucl_parser_flags_t NoMacros = UCL_PARSER_DISABLE_MACRO;
ucl_parser_flags_t NoVars = UCL_PARSER_NO_FILEVARS;
}; // NOTE: end namespace ucl::flags

namespace var {

std::map<std::string, std::string> add(const std::string& name, const std::string& value, std::map<std::string, std::string>& vm) {
    if (vm.contains(name)) {
        if (vm.at(name) == value) {
            return vm;
        } else {
            vm.insert_or_assign(name, value);
        }
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
confidant::configuration serialize(const string& path) {
    // ensure the file exists
    if (!fs::exists(fs::path(path))) {
        std::println(std::cerr, "error: path '{}' does not exist", path);
        std::exit(1);
    }
    
    // get a file stream, ensure it properly opened
    std::ifstream handle(path);
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

    bool create_dirs = false;
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
    
    if (ucl::check(input, "create-directories")
    && input["create-directories"].type() == ucl::Bool) {
        create_dirs = input["create-directories"].bool_value();
    }

    if (ucl::check(input, "repository")) {
        if (ucl::check(input["repository"], "url")) {
            repository.url = input["repository"]["url"].string_value();
        }
    } else {
        repository.url = "";
    }

    if (!ucl::check(input, "link")) {
        std::println(std::cerr, "expected field 'link' was not found!");
        std::exit(1);
    }

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

        if (!ucl::check(object, "type"))
            link.type = confidant::config::linkType::file;
        else {
            if (object["type"].string_value() == "file") link.type = confidant::config::linkType::file;
            else if (object["type"].string_value() == "directory") link.type = confidant::config::linkType::directory;
            else {
                std::println(std::cerr, "warning: type '{}' is not recognized, expected one of 'file' or 'directory'. Using default value of 'file'.", object["type"].string_value());
                link.type = confidant::config::linkType::file;
            }
        }
        links.push_back(link);
    }

    if (!ucl::check(input, "link-from")) {
        std::println(std::cerr, "expected field 'link-from' was not found!");
        std::exit(1);
    }

    auto linkfrom_ucl = input.lookup("link-from");
    
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
    
    conf.create_dirs = create_dirs;
    conf.repo = repository;
    conf.links = links;
    conf.linkFrom = linkFrom;

    return conf;

}
}; // END confidant::config
}; // END confidant