// SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "confidant.hpp"
#include "logging.hpp"
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

using std::string;

namespace options {
    
    namespace config {
        inline int verbosity = 0;
        inline bool help = false;
        inline bool quiet = false;
    
        namespace dump {
            inline bool global = false;
            inline bool help = false;
            inline int verbosity = 0;
            inline bool quiet = false;
            inline string file = fs::current_path().string() + "/confidant.ucl";
        };
    
        namespace get {
            inline bool global = false;
            inline bool help = false;
            inline int verbosity = 0;
            inline bool quiet = false;
            inline string file = fs::current_path().string() + "/confidant.ucl";
            inline string name;
        };
    
    };
    
    namespace link {
        inline bool help = false;
        inline bool dryrun = false;
        inline int verbosity = 0;
        inline bool quiet = false;
        inline string file = fs::current_path().string() + "/confidant.ucl";
    };
    
    namespace init {
        inline string path = fs::current_path().string();
        inline int verbosity = 0;
        inline bool dryrun = false;
        inline bool help = false;
        inline bool quiet = false;
    };
    
    inline bool version = false;
    inline bool usage = false;
    inline bool help = false;
    inline int verbosity = 0;
    inline bool quiet;
    
    namespace global {
        inline bool create_dirs = true;
        inline int verbosity = ansi::verbosity::normal;
        inline bool color = true;
        // inline confidant::settings defaults() {
        //     return confidant::settings{
        //         .create_dirs = create_dirs,
        //         .loglevel = verbosity,
        //         .color = color
        //     };
        // }
    };
};