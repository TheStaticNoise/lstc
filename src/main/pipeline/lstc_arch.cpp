#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

namespace lstc::arch {
  class obj_interpreter {
    private:
      struct {
        uint8_t x;
      } work_info;
      bool write_sf(int fd, const std::string& form) {
        for (uint64_t i = 0; i < form.size();) {
          int64_t e = write(fd, form.data()+i, form.size()-i);
          if (e <= 0) {return false;}

          i += e;
        }
        return true;
      } 
    public:
      uint64_t write_into(std::string path, std::vector<uint64_t> asmgen_tk) {
        int file = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
        if (file == -1) {
          return 1;
        }
        std::string header = "; made by lstc compiler, do not edit (please)\n";
        write_sf(file, header);
        close(file);
        return 0;
      }
  };
}
