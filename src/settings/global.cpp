#include "settings.hpp"

namespace confidant {
    namespace config {
        namespace global {
            confidant::settings defaults() {
                return confidant::settings{
                    .createdirs = true,
                    .color = true,
                    .loglevel = verbose::normal
                };
            }
        };
    };
    
};