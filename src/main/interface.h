#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace lstc {
    namespace FileManag {
        #ifdef __unix__
            inline std::string __lstc_fileManag_sepr = "/";
            #define POSIX_C
        #elif _WIN32
            inline std::string __lstc_fileManag_sepr = "\\";
            #define WIN_PLATFORM
        #endif


        enum PathCompType {
            HOME = 0, //- expect: None! (no index increase!) - home symbol, throw error if more than 1 (non-obligatory, you can just ignore it) \\ |
            PATH = 1, //- expect: String!                                                                                                       \\ |
            EVAR = 2, //- expect: String!                                                                                                       \\ |
            SEPR = 3  //- expect: None! a  '/' or '\'                                                                                           \\ |
        };

        struct part_unit {
            PathCompType   type;
            std::string content;
        };
        struct part_segs {
            std::vector<part_unit> obj;
        };

        struct path_expanded {
            std::string path;
            int          err;
        };

        part_segs path_dissect(std::string path); // back bone of lstc file manager path expansions
        path_expanded path_expand(std::string path, void* MAPENV);
        void print_partSegs(part_segs& rd);
}}
