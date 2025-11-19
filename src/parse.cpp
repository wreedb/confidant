// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <filesystem>
#include <fstream>
#include <sstream>
#include <optional>
#include <string_view>
#include <ucl.h>
#include <ucl++.h>
#include <format>
#include <print>

#include "confidant.hpp"
#include "util.hpp"

#include "parse.hpp"

namespace fs = std::filesystem;

std::string cwd = std::string(fs::current_path());

namespace ucl {
    namespace parsing {
        ucl::Ucl file(std::string_view path, const std::map<std::string, std::string>& vars) {
            if (!fs::exists(path)) {
                std::println(std::cerr, "file at {} does not exist", path);
                std::exit(1);
            }
            
            std::ifstream handle = std::ifstream(fs::path(path));
            
            if (!handle.is_open()) {
                std::println(std::cerr, "failed to open file at {}", path);
                std::exit(1);
            }
            
            std::stringstream buffer;
            
            buffer << handle.rdbuf();
            
            handle.close();
            
            std::string inputstr, errorstr;
            
            ucl::Ucl input = ucl::Ucl::parse(buffer.str(), vars, errorstr, UCL_DUPLICATE_APPEND);
        
            if (!errorstr.empty()) {
                std::println(std::cerr, "failed while parsing {}\n{}", path, errorstr);
                std::exit(1);
            }
            
            return input;
        }
    };
    
    namespace get {
        std::optional<bool> boolean(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::Bool)
                return object.lookup(key).bool_value();
            else
                return std::nullopt;
        }
        
        std::optional<int> integer(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::Int)
                return object.lookup(key).int_value();
            else
                return std::nullopt;
        }
        
        std::optional<string> str(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::String)
                return object.lookup(key).string_value();
            else
                return std::nullopt;
        }
        
        std::optional<ucl::Ucl> list(const ucl::Ucl& object, const std::string& key) {
            if (object.lookup(key).type() == ucl::Array)
                return object.lookup(key);
            else
                return std::nullopt;
        }
        
        std::optional<ucl::Ucl> node(const ucl::Ucl& object, const std::string& key) {
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