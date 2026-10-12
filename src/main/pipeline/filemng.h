#pragma once
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <array>
#include <string_view>

#ifdef __unix__
#include <fcntl.h>
#include <unistd.h>
#endif
namespace lstc::filemanager {
  enum class part_type : uint8_t {NONE, SEPR, EVAR, HOME, PATH};
  struct part_unit {
    part_type type;
    std::optional<std::string> data;
  };
  struct part {
    std::vector<part_unit> parts;
  };
  static part dissect(std::string_view path) {
    part ret;
    std::string collect;
    auto check = [&]() {
      if (!collect.empty()) {
        ret.parts.push_back({part_type::PATH, collect});
        collect.clear();
      }
      return;
    };

    for (uint64_t i = 0; i < path.size(); i++) {
      char symbol = path[i];
      if (symbol == '/' || symbol == '\\') {
        check();
        ret.parts.push_back({part_type::SEPR, std::nullopt}); 
        continue;
      }
      if (symbol == '$') {
        check();
        if (i + 1 < path.size() && path[i+1] == '{') {
          // advanced capture mode
          i+=2;
          for (; i < path.size() && path[i] != '}'; i++) {
            collect+=path[i];  
          }
          i++;
        } else {
          i++;
          for (; i < path.size() && (path[i] != '/' || path[i] != '\\'); i++) {
            collect+=path[i];
          }
        }
        // dump
        ret.parts.push_back({part_type::EVAR, collect});
        collect.clear();
        continue;
      }
      collect+=path[i];
    }
    check();
    return ret;
  }

  static std::string expand_path(std::string path, std::optional<std::unordered_map<std::string, std::string>> cenv, std::optional<bool> allow_direnv) {
    part e = dissect(path);
    
  }
  typedef struct file file;
  file* open_file(std::string path, int perms, int flags);
  uint64_t file_size(file* f);
  int64_t read_file(file* f, char* buf, uint64_t n);
  int create_file(std::string path);
  int64_t write(file* f, char* buf, uint64_t n);
}
