#include <cstdio>
#include <string>
#include <iostream>
#include "interface.h"
#include <unordered_map>

using namespace lstc::FileManag;

namespace lstc::FileManag {
    // worst lexer in all of programming history
    part_segs path_dissect(std::string path) {
        part_segs r;
        std::string tmp;
        int capture4var = -1; // -1 = no, x = seg
        auto handle_diversity = [&r, &tmp, &capture4var](PathCompType sep) {
            if (capture4var == -1) {
                if (!tmp.size()) {
                    r.obj.push_back({sep, ""});
                } else {
                    r.obj.push_back({PATH, tmp});
                    r.obj.push_back({sep, ""});
                    tmp.clear();
                }
            } else {
                r.obj[capture4var].content = tmp;
                capture4var = -1;
                tmp.clear();
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
                    seg++;
                    break;
                case '$':
                    if (tmp.size()) {
                        r.obj.push_back({PATH, tmp});
                        seg++;
                    }
                    r.obj.push_back({EVAR, ""});
                    capture4var = seg++;
                    break;
                default:
                    tmp += path[iter];
                    break;
            }
        }
        r.obj[seg].content = tmp;
        return r;
    }
    // unsafe!
    path_expanded path_expand(std::string path, void* MAPENV) {
        path_expanded rq;
        std::unordered_map<std::string, std::string>* MAP = (std::unordered_map<std::string, std::string>*) MAPENV;
        auto res = path_dissect(path);
        std::string home;
        std::string constructed;
        std::string tmp;
        char* temp;
        auto _env_op = [&MAP, &rq, &temp, &constructed](part_unit& r) {
            int set = 0;
            if (MAP != NULL) {
                auto e = MAP->find(r.content);
                if (e != MAP->end()) {
                    constructed += e->second;
                    set = 1;
                }
            }
            if (!set) {
                temp = std::getenv(r.content.c_str());
                if (temp == NULL) {
                    std::cerr << "[ lstc ] Wrong env var : [\"" << r.content << "\"]! [CRIT] \n";
                    rq.err = -1;
                    return rq;
                } else {
                    constructed += temp;
                }
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

        for (long i = 0; i < res.obj.size(); i++) {
            switch (res.obj[i].type) {
                case HOME:
                    temp = std::getenv("HOME");
                    break;
                case PATH:
                    constructed += res.obj[i].content;
                    break;
                case EVAR:
                    _env_op(res.obj[i]);
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
