// SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <string>
#include <filesystem>

#include "util.hpp"

namespace fs = std::filesystem;

using std::string;

namespace options {
    
    namespace config {
        inline bool invoked = false;
        inline bool verbose = false;
        inline bool help = false;
        inline bool quiet = false;
    
        namespace dump {
            inline bool invoked = false;
            inline bool global = false;
            inline bool help = false;
            inline bool verbose = false;
            inline bool quiet = false;
            inline std::string file = fs::current_path().string() + "/confidant.ucl";
        }; // END dump
    
        namespace get {
            inline bool invoked = false;
            inline bool global = false;
            inline bool help = false;
            inline bool verbose = false;
            inline bool quiet = false;
            inline std::string file = fs::current_path().string() + "/confidant.ucl";
            inline std::string name;
        }; // END get
    
    }; // END config
    
    namespace link {
        inline bool invoked = false;
        inline std::string tags;
        inline bool help = false;
        inline bool dryrun = false;
        inline bool verbose = false;
        inline bool quiet = false;
        inline std::string file = fs::current_path().string() + "/confidant.ucl";
    }; // END link
    
    namespace init {
        inline bool invoked = false;
        inline std::string path = fs::current_path().string();
        inline bool verbose = false;
        inline bool dryrun = false;
        inline bool help = false;
        inline bool quiet = false;
    }; // END init
    
    inline bool version = false;
    inline bool usage = false;
    inline bool verbose = false;
    inline bool quiet = false;
    
    namespace help {
        inline bool invoked = false;
        inline bool link = false;
        inline bool init = false;
        namespace config {
            inline bool invoked = false;
            inline bool get = false;
            inline bool dump = false;
        }; // END config
    }; // END help
    
    namespace global {
        inline bool create_dirs = true;
        inline int verbosity = util::verbose::normal;
        inline bool color = util::usecolorp();
    }; // END global
}; // END options
