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

#include "i18n.hpp"
#include "util.hpp"
#include "parse.hpp"

namespace fs = std::filesystem;

std::string cwd = std::string(fs::current_path());

namespace ucl {
    namespace parsing {
        ucl::Ucl file(std::string_view path, const std::map<std::string, std::string>& vars) {
            if (!fs::exists(path)) {
                std::cerr << _("file at") << path << _("does not exist") << std::endl;
                std::exit(1);
            }
            
            std::ifstream handle = std::ifstream(fs::path(path));
            
            if (!handle.is_open()) {
                std::cerr << _("failed to open file at ") << path << std::endl;
                std::exit(1);
            }
            
            std::stringstream buffer;
            
            buffer << handle.rdbuf();
            
            handle.close();
            
            std::string inputstr, errorstr;
            
            ucl::Ucl input = ucl::Ucl::parse(buffer.str(), vars, errorstr, UCL_DUPLICATE_APPEND);
        
            if (!errorstr.empty()) {
                std::cerr << _("failed while parsing ") << path << "\n" << errorstr << std::endl;
                std::exit(1);
            }
            
            return input;
        }
    };
    
    namespace get {
        std::optional<bool> boolean(const ucl::Ucl& object, std::string_view key) {
            if (object.lookup(key.data()).type() == ucl::Bool)
                return object.lookup(key.data()).bool_value();
            else return std::nullopt;
        }
        
        std::optional<int> integer(const ucl::Ucl& object, std::string_view key) {
            if (object.lookup(key.data()).type() == ucl::Int)
                return object.lookup(key.data()).int_value();
            else return std::nullopt;
        }
        
        std::optional<std::string> str(const ucl::Ucl& object, std::string_view key) {
            if (object.lookup(key.data()).type() == ucl::String)
                return object.lookup(key.data()).string_value();
            else return std::nullopt;
        }
        
        std::optional<ucl::Ucl> list(const ucl::Ucl& object, std::string_view key) {
            if (object.lookup(key.data()).type() == ucl::Array)
                return object.lookup(key.data());
            else return std::nullopt;
        }
        
        std::optional<ucl::Ucl> node(const ucl::Ucl& object, std::string_view key) {
            if (object.lookup(key.data()).type() == ucl::Object)
                return object.lookup(key.data());
            else return std::nullopt;
        }
    };

    namespace var {

        void add(std::string_view name, std::string_view value, std::map<std::string, std::string>& vm) {
            vm.insert_or_assign(std::string(name), std::string(value));
        }

        void fromenv(std::string_view key, std::map<std::string, std::string>& vm, std::string_view fallback) {
            std::optional<std::string> env = util::getenv(key.data());
            if (!env) ucl::var::add(key, fallback, vm);
            else ucl::var::add(key, env.value(), vm);
        }
    
    }; // END ucl::var

    bool check(const ucl::Ucl& obj, std::string_view key) {
        if (obj.lookup(key.data()).type() == UCL_NULL)
            return false;
        else return true;
    }

    int members(const ucl::Ucl& object) {
        if (object.type() != UCL_OBJECT) return 0;
        int n = 0;
        for ([[maybe_unused]] const auto& _ : object) n++;
        return n;
    }

}; // END ucl