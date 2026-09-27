#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <print>
#include <cstring>
#include <csignal>


// welcome to one of the most insane codebases (In a bad way)

namespace lstc::cli {

    enum class param_toks : uint8_t{
        INPUT,
        OUTPUT,
        VERSION,
        DEBUG,
        HELP,
        NO_WARNINGS,
        STRICT, // More errors
        EMIT,
        ENVSET,
        VERBOSE,
        INCLUDE_DIR,
        PEDANTIC, // warning = error
        LIB_DIR
    };

    #define LSTCBM_INPUT 1
    #define LSTCBM_OUTPUT 2
    #define LSTCBM_VERSION 4
    #define LSTCBM_DEBUG 8
    #define LSTCBM_NOWARNINGS 16
    #define LSTCBM_STRICT 32
    #define LSTCBM_ENVSET 64
    #define LSTCBM_VERBOSE 128
    #define LSTCBM_HELP 256
    #define LSTCBM_EMIT 512
    #define LSTCBM_INCLUDE_DIR 1024 // 11th bit (1-based index, 10th position 0-based)
    #define LSTCBM_PEDANTIC 2048
    #define LSTCBM_LIB_DIR 4096
    #define LSTCBM_TEST 8192
    #define LSTCBM_INPUTEXPECTED 16384
    #define LSTCBM_OUTPUTEXPECTED 32768
    #define LSTCBM_MALFORMED 65536
    struct Param_ret {
        uint64_t bitmask;
        std::vector<std::string> input;
        std::string output;
    };

    Param_ret arg_match(std::vector<std::string> args) {
        Param_ret q{.bitmask = 0};
        std::unordered_map<std::string, int> long_names = {
            {"--input", LSTCBM_INPUTEXPECTED},
            {"--output", LSTCBM_OUTPUTEXPECTED},
            {"--version", LSTCBM_VERSION},
            {"--debug", LSTCBM_DEBUG},
            {"--wnone", LSTCBM_NOWARNINGS},
            {"--test", LSTCBM_TEST},
            {"--help", LSTCBM_HELP}
        };
        unsigned int await = 0;
        for (int i = 0; i < args.size(); i++) {
            if (args[i].starts_with("--")) {
                std::transform(args[i].begin(), args[i].end(), args[i].begin(),
                    [](unsigned char c) { return std::tolower(c); });
                auto shit = long_names.find(args[i]);
                if (shit != long_names.end()) {
                    q.bitmask |= shit->second;
                    if (shit->second == LSTCBM_INPUTEXPECTED || shit->second == LSTCBM_OUTPUTEXPECTED) await = shit->second;
                    continue;
                }
                if (args[i] == "--gfy") {
                    std::cout << "no, you gfy!\n[lstc] crit";
                    std::raise(SIGSEGV);
                }
                std::cout << "Unknown option: " << args[i] << std::endl;
                q.bitmask |= LSTCBM_MALFORMED;
            } else if (args[i][0] == '-') {
                for (int a = 1; a < args[i].length(); a++) {
                    switch (tolower(args[i][a])) {
                        case 'i':
                            q.bitmask |= LSTCBM_INPUTEXPECTED;
                            await = LSTCBM_INPUTEXPECTED;
                            break;
                        case 'o':
                            q.bitmask |= LSTCBM_OUTPUTEXPECTED;
                            await = LSTCBM_OUTPUTEXPECTED;
                            break;
                        case 't':
                            q.bitmask |= LSTCBM_TEST;
                            break;
                        case 'h':
                            q.bitmask |= LSTCBM_HELP;
                            break;
                        case 'v':
                            q.bitmask |= LSTCBM_VERSION;
                            break;
                        default:
                            std::cout << "Unknown option -" << args[i][a] << std::endl;
                            q.bitmask |= LSTCBM_MALFORMED;
                    }
                }
            } else {
                if (await == LSTCBM_INPUTEXPECTED) {
                    q.input.push_back(args[i]);
                    await = 0;
                    q.bitmask ^= LSTCBM_INPUTEXPECTED;
                    q.bitmask |= LSTCBM_INPUT;
                } else if (await == LSTCBM_OUTPUTEXPECTED) {
                    q.output = args[i];
                    q.bitmask   ^= LSTCBM_OUTPUTEXPECTED;
                    q.bitmask   |= LSTCBM_OUTPUT;
                    await = 0;
                }
            }
        }
        return q;
    }

    void print_help() {
        std::cout <<
        "[lstc] usage: lstc <params>\n" <<
        "- options&params\n" <<
        "[=][=][=][=][=][=]\n" <<
        "* pedantic (-p/--pedantic)\n" <<
        "! Makes any warning stop compilation\n" <<
        "* emit (-e/--emit) | usage: <stage>\n" <<
        "! emits stage <stage>\n" <<
        "* input files (-i/--input) | usage: <files>\n" <<
        "! sets input files\n" <<
        "* output (-o/--output) | usage: <file>\n" <<
        "! set output file\n" <<
        "[=]========[=]\n" <<
        "example: \n    lstc -p -i file.lstc file2.lstc -o a.out\n" <<
        "[lstc] Info\n" <<
        "License: MIT\n" <<
        "Author: static_noise\n" <<
        "project: lstc compiler\n" <<
        "github: https://github.com/TheStaticNoise/lstc\n";
    }
}

using namespace lstc::cli;

int main(int argc, char** argv, char** envp) {
    std::vector<std::string> vec_arg;
    std::cout << "[lstc] startup\n";
    for (auto i = 1; i < argc; i++) {
        vec_arg.push_back(argv[i]);
    }
    auto e = arg_match(vec_arg);
    if (e.bitmask & LSTCBM_MALFORMED) {
        std::cout << "[lstc] Invalid options!\n";
        return -1;
    }
    if (e.bitmask & LSTCBM_HELP) {
        print_help();
    }
    return 0;
}
