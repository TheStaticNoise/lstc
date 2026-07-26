#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace lstc {
    namespace FileManag {
        enum PathCompType {
            HOME, // expect: no String (no index increase!) - home symbol, throw error if more than 1 (non-obligatory, you can just ignore it)
            PATH, // expect: String!
            EVAR  // expect: String!
        };
        struct part_segs {
            std::vector<PathCompType> type;
            std::vector<std::string>   obj;
        };
        PathCompType path_dissect(std::string path); // back bone of lstc file manager path expansions
        std::string path_expand(std::string path);
        std::string path_expand_Cenv(std::string path, std::unordered_map<std::string, std::string> env_vars);
}}
