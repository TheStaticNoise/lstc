#pragma once

#include <cerrno>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <optional>
#include <pthread.h>
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
      bool write_sf(int fd, const std::string_view& form) {
        for (uint64_t i = 0; i < form.size();) {
          int64_t e = write(fd, form.data()+i, form.size()-i);
          if (e < 0) {if (errno = EINTR) {continue} return false;}
          if (e == 0) {return false;}

          i += e;
        }
        return true;
      } 
    public:
      uint64_t write_into(std::string path, std::vector<uint64_t> asmgen_tk) {
        int file = open(path.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
        if (file <= 0) {
          return 1;
        }
        std::string header =
          "\t; made by lstc compiler, do not edit (please)\n"
          "\t; boring parts\n" 
          "\t; compiler: lstc compiler by static_noise (@static_noise/lstc: github)\n"
          "\t; codegen version: " "0.0.1" "\n"
          "\t; lstc compiler license: MIT\n"
          "\t; Warning! The license of the compiler may differ from the license of the compiled app\n"; // all lstc compiled apps are GN- nope, not gnu, never, not any license, no forcing fucking licenses on ppl from the compiler        
          
        // here will be the logic


        if (!write_sf(file, header)) {close(file); return -1;}
        close(file);
        return 0;
      }
  };
}
