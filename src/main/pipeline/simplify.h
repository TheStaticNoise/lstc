#pragma once

#include "lexer.h"
#include <cstdint>
#include <optional>
#include <string_view>
// enum class status:uint8_t {SINGLE_AF, DATING, MARRIED, DEAD};
// status me = status::SINGLE_AF;
// status you = status::SINGLE_AF; // aka anybody who reads this code most probably

namespace lstc::simplify {
  class object {
    private:
      lstc::lexer::lex* lexer = nullptr;
      std::vector<lstc::lexer::file_data> files;
    public:
      struct directive {
        enum class DIR_TYPE: uint8_t {NONE = 0, INCLUDE = static_cast<uint8_t>(lstc::lexer::token_value::INCLUDE), ENTRY = static_cast<uint8_t>(lstc::lexer::token_value::ENTRY_TAG)} type = DIR_TYPE::NONE;
        uint64_t identity_id = 0;
      };
      inline void set_lexerInstance(lstc::lexer::lex& lex) {
        lexer = &lex;
      }
      int64_t loadfile(std::string& filename, uint64_t* id) {
        if (!id || !lexer) {return-1;}
        auto fileid = lexer->find_file(std::string_view(filename.c_str()));
        if (!fileid.has_value()) return -2;

        files.push_back(lexer->get_file(fileid.value()));
        *id = files.size()-1;
        return 0;
      }
      std::optional<std::vector<directive>> dirAnalyze(uint64_t id, uint64_t* err) {
        if (!err) {return std::nullopt;}
        if (files.size() <= id) {*err = 1; return std::nullopt;}
        *err = 0;
        auto& file = files[id];
        auto& token_stream = file.tokens;
        
        std::vector<directive> dirs;

        lstc::lexer::token_value direct = lstc::lexer::token_value::ZERO;

        for (auto token : token_stream) {
          // iterate over all
          if (token == lstc::lexer::token_value::ENTRY_TAG || token == lstc::lexer::token_value::INCLUDE) {
            if (direct == lstc::lexer::token_value::ZERO) {
              direct = token; continue;
            }
          }
          if (token >= lstc::lexer::token_value::IDENTITY_BASE && direct != lstc::lexer::token_value::ZERO) {
            dirs.push_back(
                {
                  static_cast<directive::DIR_TYPE>(direct), 
                  static_cast<uint64_t>(token) - static_cast<uint64_t>(lstc::lexer::token_value::IDENTITY_BASE)
                }
              );
            direct = lstc::lexer::token_value::ZERO;
            continue;
          } 
          if (direct == lstc::lexer::token_value::ZERO) {
            continue;
          } else if (token < lstc::lexer::token_value::IDENTITY_BASE) {
            *err = 2;
            return std::nullopt;
          }
          else if (direct != lstc::lexer::token_value::ZERO) {
            *err = 3;
            return std::nullopt;
          }
        }
        if (direct != lstc::lexer::token_value::ZERO) {
          *err = 4;
          for (auto dir : dirs) {
            std::cout <<
              "Type: " << static_cast<uint64_t>(dir.type) <<
              "\nIdent: " << static_cast<uint64_t>(dir.identity_id) <<
              std::endl;
          }
          return std::nullopt;
        }
        return dirs;
      }
  };
}
