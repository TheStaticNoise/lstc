#include <cstdio>
#include <string>
#include <iostream>
#include "interface.h"
#include <unordered_map>
#include <optional>

using namespace lstc::FileManag;

namespace lstc::FileManag {
    // worst lexer in all of programming history
    part_segs path_dissect(std::string path) {
        part_segs r;
        r.error = 0;
        std::string tmp;
        part_unit* pt = NULL;
        int capture4var = -1; // -1 = no, x = seg
        auto handle_diversity = [&pt, &r, &tmp, &capture4var](PathCompType sep) {
            if (capture4var == -1) {
                if (!tmp.size()) {
                    r.obj.push_back({sep, ""});
                } else {
                    r.obj.push_back({PATH, tmp});
                    r.obj.push_back({sep, ""});
                    tmp.clear();
                }
            } else {
                if (!pt) {
                    r.error = -1;
                } else {
                    pt->content = tmp;
                    capture4var = -1;
                    pt = NULL;
                    tmp.clear();
                }
            }
        };
        // work damit!
        int seg = 0;
        for (long iter = 0; iter < path.size(); iter++) {
            switch (path[iter]) {
                case '~':
                    handle_diversity(HOME);
                    seg++;
                    break;
                case '/':
                case '\\':
                    handle_diversity(SEPR);
                    break;
                case '$':
                    if (tmp.size()) {
                        r.obj.push_back({PATH, tmp});
                    }
                    r.obj.push_back({EVAR, ""});
                    pt = &r.obj[r.obj.size() - 1];
                    capture4var = 1;
                    break;
                default:
                    tmp += path[iter];
                    break;
            }
        }
        r.obj[r.obj.size()-1].content = tmp;
        return r;
    }
    // unsafe! anyone passing integers, strings, sets, his own relationship history (aka 1, in kindergarden but then never again), instead of null, nullptr, or unordered map
    // will be executed at sunrise
    path_expanded path_expand(std::string path, std::optional<std::unordered_map<std::string, std::string>> MAPENV) {
        path_expanded rq;
        auto MAP = MAPENV.value();
        auto evarNeed = 1;
        auto res = path_dissect(path); // problem is here
        std::string home;
        std::string constructed;
        std::string tmp;
        char* temp;
        auto _env_op = [&evarNeed, &MAP, &MAPENV, &rq, &temp, &constructed](part_unit* r) {
            if (!evarNeed) return 0;
            int set = 0;
            if (MAPENV.has_value()) {
                auto e = MAP.find(r->content);
                if (e != MAP.end()) {
                    constructed += e->second;
                    set = 1;
                    return 1;
                }
            }
            if (!set) {
                temp = std::getenv(r->content.c_str());
                if (temp == NULL) {
                    std::cerr << "[ lstc ] Wrong env var : [\"" << r->content << "\"]! [CRIT] \n";
                    rq.err = -1;
                } else {
                    constructed += temp;
                    return 1;
                }
            }
            return 0;
        };

        auto _add_home = [&constructed, &rq, &temp]() {
            temp = std::getenv("HOME");
            if (temp == NULL) {
                std::cerr << "No home found! (You're homeless XD)\n";
                rq.err = -1;
            } else {
                constructed += temp;
            }
        };

        auto _decode_type = [](std::string& tmp, PathCompType type) {
            switch (type) {
                case HOME:
                    tmp = "Home";
                    break;
                case PATH:
                    tmp = "Path";
                    break;
                case EVAR:
                    tmp = "Env Variable";
                    break;
                case SEPR:
                    tmp = "Separator";
                    break;
                default:
                    tmp = "Unknown type";
            }
        };
        std::unordered_map<std::string, std::string>::iterator e;
        for (long i = 0; i < res.obj.size(); i++) {
            switch (res.obj[i].type) {
                case HOME:
                    _add_home();
                    break;
                case PATH:
                    constructed += res.obj[i].content;
                    break;
                case EVAR:
                    std::cerr << "EVAR\n";
                    if (MAPENV.has_value()) {
                        e = MAP.find(res.obj[i].content);
                        if (e == MAP.end()) {
                            std::cerr << "Could not find var " << res.obj[i].content << " in map!\n";
                            temp = std::getenv(res.obj[i].content.c_str());
                            if (!temp) {
                                std::cerr << "[ lstc ] Wrong env var : [\"" << res.obj[i].content << "\"]! [CRIT] \n";
                                rq.err = -1;
                                return rq;
                            }
                            constructed += temp;
                        } else {
                            constructed += e->second;
                        }
                    }
                    break;
                case SEPR:
                    constructed += __lstc_fileManag_sepr;
                    break;
                default:
                    _decode_type(tmp, res.obj[i].type);
                    std::cerr << "[ lstc ] Bad result! = [" << tmp << "] : [" << res.obj[i].content << "]\n";
                    rq.err = -2;
                    return rq;
            }
            if (rq.err) return rq;
        }
        rq.path = constructed;
        return rq;
    };

    void print_partSegs(part_segs& rd) {
        std::string tmp;
        for (long i = 0; i < rd.obj.size(); i++) {
            switch (rd.obj[i].type) {
                case HOME:
                    tmp = "Home";
                    break;
                case PATH:
                    tmp = "Path";
                    break;
                case EVAR:
                    tmp = "Env Variable";
                    break;
                case SEPR:
                    tmp = "Separator";
                    break;
                default:
                    tmp = "Unknown type";
            }
        std::cout << tmp << " : " << rd.obj[i].content << "\n";
        }
        fflush(stdout);
    }
}
