#include <string>
#include <filesystem>

namespace fs = std::filesystem;

using std::string;

namespace options {
    
    namespace config {
        inline int verbosity = 0;
        inline bool help = false;
    
        namespace dump {
            inline bool global = false;
            inline bool help = false;
            inline int verbosity = 0;
            inline string file = fs::current_path().string();
        };
    
        namespace get {
            inline bool global = false;
            inline bool help = false;
            inline int verbosity = 0;
            inline string file = fs::current_path().string() + "/confidant.ucl";
            inline string name;
        };
    
    };
    
    namespace link {
        inline bool help = false;
        inline bool dryrun = false;
        inline int verbosity = 0;
        inline string file = fs::current_path().string() + "/confidant.ucl";
    };
    
    namespace linkfrom {
        inline bool help = false;
        inline bool dryrun = false;
        inline int verbosity = 0;
        inline string file = fs::current_path().string() + "/confidant.ucl";
    };
    
    namespace init {
        inline string path = fs::current_path().string();
        inline int verbosity = 0;
        inline bool dryrun = false;
        inline bool help = false;
    };
    
    inline bool version = false;
    inline bool usage = false;
    inline bool help = false;
    inline int verbosity = 0;
};