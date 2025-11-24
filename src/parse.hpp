// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <string_view>
#include <ucl++.h>
#include <ucl.h>


using std::optional;

namespace ucl {
    
    inline ucl_type_t Null = UCL_NULL;
    inline ucl_type_t Object = UCL_OBJECT;
    inline ucl_type_t Array = UCL_ARRAY;
    inline ucl_type_t Int = UCL_INT;
    inline ucl_type_t Float = UCL_FLOAT;
    inline ucl_type_t String = UCL_STRING;
    inline ucl_type_t Bool = UCL_BOOLEAN;
    inline ucl_type_t Time = UCL_TIME;
    inline ucl_type_t UserData = UCL_USERDATA;
    
    namespace emit {
        inline ucl_emitter json = UCL_EMIT_JSON;
        inline ucl_emitter jsonc = UCL_EMIT_JSON_COMPACT;
        inline ucl_emitter yaml = UCL_EMIT_YAML;
        inline ucl_emitter yml = UCL_EMIT_YAML;
        inline ucl_emitter msgpack = UCL_EMIT_MSGPACK;
        inline ucl_emitter config = UCL_EMIT_CONFIG;
        inline ucl_emitter max = UCL_EMIT_MAX;
    }; // END ucl::emit

    namespace flags {
        inline ucl_parser_flags_t Default = UCL_PARSER_DEFAULT;
        inline ucl_parser_flags_t LowercaseKeys = UCL_PARSER_KEY_LOWERCASE;
        inline ucl_parser_flags_t ZeroCopy = UCL_PARSER_ZEROCOPY;
        inline ucl_parser_flags_t NoTime = UCL_PARSER_NO_TIME;
        inline ucl_parser_flags_t ExplicitArrays = UCL_PARSER_NO_IMPLICIT_ARRAYS;
        inline ucl_parser_flags_t SaveComments = UCL_PARSER_SAVE_COMMENTS;
        inline ucl_parser_flags_t NoMacros = UCL_PARSER_DISABLE_MACRO;
        inline ucl_parser_flags_t NoVars = UCL_PARSER_NO_FILEVARS;
    }; // END ucl::flags
    
    namespace parsing {
        ucl::Ucl file(std::string_view path, const std::map<std::string, std::string>& vars);
    };

    namespace get {
        std::optional<bool> boolean(const ucl::Ucl& object, const std::string& key);
        std::optional<int> integer(const ucl::Ucl& object, const std::string& key);
        std::optional<std::string> str(const ucl::Ucl& object, const std::string& key);
        std::optional<ucl::Ucl> list(const ucl::Ucl& object, const std::string& key);
        std::optional<ucl::Ucl> node(const ucl::Ucl& object, const std::string& key);
    }; // END ucl::get

    namespace var {
        std::map<std::string, std::string> add(const std::string& name, const std::string& value, std::map<std::string, std::string>& vm);
        std::map<std::string, std::string> fromenv(const std::string& key, std::map<std::string, std::string>& vm, const std::string& fallback);
    }; // END ucl::var

    bool check(const ucl::Ucl& obj, const std::string& key);
    int members(const ucl::Ucl& object);

}; // END ucl