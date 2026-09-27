#include <iostream>
#include <array>
#include <cstring>
#include <pthread.h>
#include <cstdlib>
#include <string>
#include <string_view>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <optional>
#include <variant>
/*
#include <llvm/sucks>
*/


namespace lstc::lexer {
  enum class token_value: uint64_t {
    // generals
    // ZERO: no token
    ZERO, FN, LET, DESCRIPTOR, CURLY_LEFT, CURLY_RIGHT, ENTRY_TAG, DOT, SEMICOLON, COLON, CLASS_DESCRIPTOR, COMMA, RETURN,
    // conditionals & branches
    IF, ELSE, FOR, WHILE,
    // operands
    OP_ASTERISK, OP_SLASH, OP_PLUS, OP_MINUS, OP_XOR /* ^ */, OP_QUESTION, OP_EXCLAM, OP_PIPE, OP_AND, OP_EQUALS,
    // types
    TYPE_I8, TYPE_I16, TYPE_I32, TYPE_I64, TYPE_U8, TYPE_U16, TYPE_U32, TYPE_U64, TYPE_CHAR, TYPE_NONE, TYPE_STR, TYPE_F32, TYPE_F64,
    TYPE_BOOL,
    // bool values
    VALUE_TRUE, VALUE_FALSE, 
    // braces 
    ROUND_LEFT, ROUND_RIGHT, SQUARE_LEFT, SQUARE_RIGHT, ANGLE_LEFT, ANGLE_RIGHT, NEWLINE, /* this token will be ignored, used ONLY for error prints */
    // specials
    INCLUDE,
    // token end
    IDENTITY_BASE
  };


  // special map to decode
  std::unordered_map<token_value, std::string_view> tokens_to_string = {
    {token_value::ZERO, "no token"},
    {token_value::FN, "fn"},
    {token_value::LET, "let"},
    {token_value::DESCRIPTOR, "->"},
    {token_value::CURLY_LEFT, "{"},
    {token_value::CURLY_RIGHT, "}"},
    {token_value::ENTRY_TAG, "#entry"},
    {token_value::DOT, "."},
    {token_value::SEMICOLON, ";"},
    {token_value::COLON, ":"},
    {token_value::CLASS_DESCRIPTOR, "::"},
    {token_value::COMMA, ","},
    {token_value::RETURN, "return"},
    {token_value::IF, "if"},
    {token_value::ELSE, "else"},
    {token_value::FOR, "for"},
    {token_value::WHILE, "while"},
    {token_value::OP_ASTERISK, "*"},
    {token_value::OP_SLASH, "/"},
    {token_value::OP_PLUS, "+"},
    {token_value::OP_MINUS, "-"},
    {token_value::OP_XOR, "^"},
    {token_value::OP_QUESTION, "?"},
    {token_value::OP_EXCLAM, "!"},
    {token_value::OP_PIPE, "|"},
    {token_value::OP_AND, "&"},
    {token_value::OP_EQUALS, "="},
    {token_value::TYPE_I8, "i8"},
    {token_value::TYPE_I16, "i16"},
    {token_value::TYPE_I32, "i32"},
    {token_value::TYPE_I64, "i64"},
    {token_value::TYPE_U8, "u8"},
    {token_value::TYPE_U16, "u16"},
    {token_value::TYPE_U32, "u32"},
    {token_value::TYPE_U64, "u64"},
    {token_value::TYPE_CHAR, "char"},
    {token_value::TYPE_NONE, "none"},
    {token_value::TYPE_STR, "str"},
    {token_value::TYPE_F32, "f32"},
    {token_value::TYPE_F64, "f64"},
    {token_value::TYPE_BOOL, "bool"},
    {token_value::VALUE_TRUE, "true"},
    {token_value::VALUE_FALSE, "false"},
    {token_value::ROUND_LEFT, "("},
    {token_value::ROUND_RIGHT, ")"},
    {token_value::SQUARE_LEFT, "["},
    {token_value::SQUARE_RIGHT, "]"},
    {token_value::ANGLE_LEFT, "<"},
    {token_value::ANGLE_RIGHT, ">"},
    {token_value::NEWLINE, "\\n"},
    {token_value::INCLUDE, "#include"}
  };
  std::unordered_map<std::string, token_value> vocab = {
    // generics
    {"#entry", token_value::ENTRY_TAG}, {"#include", token_value::INCLUDE}, {"fn", token_value::FN}, {"let", token_value::LET}, {"return", token_value::RETURN},
    // symbols
    {"->", token_value::DESCRIPTOR}, {"{", token_value::CURLY_LEFT}, {"}", token_value::CURLY_RIGHT},
    {".", token_value::DOT}, {",", token_value::COMMA}, {"!", token_value::OP_EXCLAM},
    {"[", token_value::SQUARE_LEFT}, {"]", token_value::SQUARE_RIGHT}, {"(", token_value::ROUND_LEFT}, {")", token_value::ROUND_RIGHT},
    {"<", token_value::ANGLE_LEFT},{">", token_value::ANGLE_RIGHT}, {":", token_value::COLON}, {"::", token_value::CLASS_DESCRIPTOR}, {";", token_value::SEMICOLON},
    // types
    {"i8", token_value::TYPE_I8},{"i16", token_value::TYPE_I16},{"i32", token_value::TYPE_I32},{"i64", token_value::TYPE_I64},
    {"u8", token_value::TYPE_U8},{"u16", token_value::TYPE_U16},{"u32", token_value::TYPE_U32},{"u64", token_value::TYPE_U64},
    {"f32", token_value::TYPE_F32}, {"f64", token_value::TYPE_F64},
    {"bool", token_value::TYPE_BOOL},{"char",token_value::TYPE_CHAR},{"none", token_value::TYPE_NONE}, {"str", token_value::TYPE_STR},
    //values 
    {"true", token_value::VALUE_TRUE}, {"false", token_value::VALUE_FALSE},
    // expr & ops
    {"+", token_value::OP_PLUS}, {"-", token_value::OP_MINUS}, {"/", token_value::OP_SLASH}, {"*", token_value::OP_ASTERISK}, {"^", token_value::OP_XOR},
    {"?", token_value::OP_QUESTION}, {"=", token_value::OP_EQUALS}, {"|", token_value::OP_PIPE}, {"&", token_value::OP_AND},
    // branches
    {"if", token_value::IF}, {"else", token_value::ELSE}, {"for", token_value::FOR}, {"while", token_value::WHILE}
  }; 

  struct identity {
    enum {identity_identifier, identity_literal_int, identity_literal_str, identity_literal_float} kind;
    std::variant<std::string, uint64_t, double> data; 
  };
  struct file_data {
    std::vector<token_value> tokens;
    std::string name, path;
    bool parsed = false;
    file_data(std::string n, std::optional<std::string> p) : name(n), path(p.value_or("")) {} 
  };

  class lex {
    private: 
      struct {
        std::vector<file_data> files;
        std::unordered_map<std::string, uint64_t> file_map;
        std::vector<file_data> data;
        std::vector<identity> iden;
        std::unordered_map<std::string, uint64_t> identifier_map;
      } work_info;
      int new_file(std::string_view name, std::optional<std::string_view> path) {
        file_data data(std::string(name), std::string(path.value_or("")));
        work_info.files.push_back(std::move(data));
        work_info.file_map.insert({std::string(name), work_info.files.size()-1});
        return work_info.files.size()-1;
      }
    public:
      file_data& get_file(uint64_t id) {
        return work_info.files.at(id);
      }
      std::optional<uint64_t> find_file(const std::string_view name) {
        auto e = work_info.file_map.find(std::string(name));
        if (e == work_info.file_map.end())
          return std::nullopt;
        return e->second;
      }
      bool file_exists(uint64_t id) {
        return id < work_info.files.size();
      }
      std::optional<uint64_t> lex_string(std::string_view source, std::string_view name, std::optional<std::string_view> path) {
        enum class types {NONE, CHAR, NUM, SPECIAL, SPACE, NEWLINE, DQUOTE, QUOTE};
        static constexpr auto lut_init = [] {
          std::array<types, 256> t{};
          for (uint64_t i = 0; i < 256; i++) {
            if (i == '\n') {
              t[i] = types::NEWLINE;
            } else if (i == ' ' || (i >= '\t' && i <= '\r')) {
              t[i] = types::SPACE;
            } else if ((i >= 'A' && i <= 'Z') || (i >= 'a' && i <= 'z') || (i == '#' || i == '$' || i == '_')) {
              t[i] = types::CHAR;
            } else if (i >= '0' && i <= '9') {
              t[i] = types::NUM;
            } else if (i == '"') {
              t[i] = types::DQUOTE; 
            } else if (i == '\'') { 
              t[i] = types::QUOTE;
            } else if (i == 0) {
              t[i] = types::NONE;
            } else {
              t[i] = types::SPECIAL;
            }
          }
          return t;
        }();

        auto lookup = [](char i) {
          return lut_init[static_cast<uint8_t>(i)];
        };

        auto bit = [](types t) {
          return (1 << static_cast<uint8_t>(t));
        };
        // none char num spec space newline dquote quote 
        static constexpr auto morpht = [&] {
          std::array<uint8_t, 8> e = {
            0,
            bit(types::CHAR) | bit(types::NUM), // char
            bit(types::NUM) | bit(types::CHAR), // num
            bit(types::SPECIAL), // spec
            0, // space
            0, // newline
            0xFF ^ bit(types::NEWLINE),
            0xFF ^ bit(types::NEWLINE)
          };
          return e;
        }();

        enum {COL_NONE, COL_ALPHANUM, COL_LITERAL, COL_QUOTE, COL_DQUOTE, COL_MAX_CRUNCH} col = COL_NONE;
        auto transform = [&](types a, types b) {
          return (morpht[static_cast<uint64_t>(a)] & bit(b)) != 0;
        };

        types a = types::NONE, b;
        bool escape=false;
        std::string collector;
        uint64_t gfl = 0; // global fault lock, if != 0 then = lexer fault 

        auto dump_litDQ = [&]() {
          work_info.iden.push_back({identity::identity_literal_str, collector});
          collector.clear();
        };
        auto dump_litQ = [&]() {
          if (collector.size() <= 8) { // max allowed size
            uint64_t ret;
            std::memcpy(&ret, collector.data(), collector.size());
            return ret;
          }
          gfl = 2;
          return (uint64_t) 0;
        };

        uint8_t escape_lifetime=0;
        uint64_t i = 0;
        const auto fid = new_file(name, path);
        auto& file = work_info.files[fid];
        auto max_munch = [&]() {
          auto end = vocab.end();
          uint64_t rewound = 0;

          while (!collector.empty()) {
            auto e = vocab.find(collector);

            if (e == end) {
              collector.pop_back();

              if (i == 0) {
                gfl = 3;
                std::cerr << "[lstc] lexer fault! i == 0 true on fallback attempt [CRIT]\n";
                return;
              }

              i--;
              rewound++;
              continue;
            }

            file.tokens.push_back(e->second);
            collector.clear();

            if (rewound)
              i--;

            break;
          }
        };
        // 2 mode remaining: 
        // directive // unobligatory 4 now
        // int/float/hex/binary literal parsing (aka just literal)

        auto res_lit = [&]() -> std::optional<uint64_t> {
          // here we go
          // res loop to get type
          enum {TP_NONE, TP_FLOAT, TP_HEX, TP_BIN, TP_INT} type = TP_NONE;

          if (collector.empty()) {
            std::cerr << "lexer fault! passed literal src size is 0 [CRIT]\n";
            gfl = 4;
            return std::nullopt;
          }

          auto assert_err = [&]() {
            if (type != TP_NONE) {
              std::cerr << "lexer fault! literal containing multiple type traits [CRIT]\n";
              gfl = 1;
              return 0;
            }
            return 1;
          };
          for (uint64_t i = 0; i < collector.size(); i++) {
            switch (collector[i]) {
              case '.':
                if (assert_err()) {
                  type = TP_FLOAT;
                  break;
                }
                return std::nullopt;
              case 'x':
                if (assert_err()) {
                  type = TP_HEX;
                  break;
                }
                return std::nullopt;
              case 'b':
                if (assert_err()) {
                  type = TP_BIN;
                  break;
                }
                return std::nullopt;
              default:
                break;
            }
          }
          // type resolved

          uint64_t base;
          char target;
          switch (type) {
            case (TP_HEX):
              base = 16;
              target = 'x';
              break;
            case (TP_FLOAT): 
              base = 0; // special marker
              target = 0;
              break;
            case (TP_BIN):
              base = 2;
              target = 'b';
              break;
            case (TP_NONE):
              type = TP_INT;
              base = 10;
              target = 0;
              break;
            default: 
              base = 10; //default to int
              target = 0;
              break;
          }
          // time for cleanup
          std::string local_copy;
          if (target != 0) {
            // needs cleanup
            uint64_t sub = 0;
            for (; sub < collector.size(); sub++) {
              if (collector[sub] == target) {
                break;
              }
            }
            if (sub == collector.size()) {
              std::cerr << "lexer fault! literal cleanup results in literal src size 0 [CRIT] \n";
              gfl = 5;
              return std::nullopt;
            }
            // sub escaped
            local_copy = collector.substr(sub);
          } else {
            local_copy = collector;
          }
          // resolve

          if (base == 0) {
            // float handling
            double db = std::strtod(local_copy.c_str(), nullptr);
            work_info.iden.push_back({identity::identity_literal_float, db});
          } else {
            // everything else
            uint64_t db = std::strtoull(local_copy.c_str(), nullptr, base);
            work_info.iden.push_back({identity::identity_literal_int, db});
          }
          collector.clear();
          return (work_info.iden.size() - 1);
        }; // DONE!!!
        auto reg_lit = [&]() {
          auto e = res_lit();
          if (!e.has_value()) return 0;
          file.tokens.push_back(static_cast<token_value>(static_cast<uint64_t>(token_value::IDENTITY_BASE) + e.value()));
          return 1; 
        };
        auto insert_iden = [&]() {
          auto e = vocab.find(collector);
          if (e != vocab.end()) {
            file.tokens.push_back(e->second);
            return;
          }
          // registring a identifier
          auto d = work_info.identifier_map.find(collector); // check if exists
          if (d != work_info.identifier_map.end()) {
            // just push back
            uint64_t val = static_cast<uint64_t>(token_value::IDENTITY_BASE)+static_cast<uint64_t>(d->second); 
            file.tokens.push_back(static_cast<token_value>(val)); // ... fucking hate casts
            return;
          };
          work_info.iden.push_back({identity::identity_identifier, collector});
          work_info.identifier_map.insert({collector, work_info.iden.end() - work_info.iden.begin()});
          uint64_t val = static_cast<uint64_t>(token_value::IDENTITY_BASE)+static_cast<uint64_t>(work_info.iden.size() - 1); 
          file.tokens.push_back(static_cast<token_value>(val));
        };

        auto step_iterator = [&](char symbol) -> std::optional<int64_t> { // returns gfl (4fun)
          b = lookup(symbol);
          if (symbol == '\\') {escape=true; escape_lifetime=0;}
          if (escape_lifetime > 1) {escape=false; escape_lifetime=0;}
          if (col == COL_DQUOTE) {
            // requires intervention, we start ALREADY from 1 past of the quote, so we can easily 
            if (b == types::DQUOTE && !escape) {
              dump_litDQ();
              a = types::NONE; // swap
              return std::nullopt;
            }
            collector += symbol;
          }
          if (col == COL_QUOTE) {
            if (b == types::QUOTE && !escape) {
              dump_litQ();
              a = types::NONE;
              return std::nullopt;
            }
            collector += symbol;
          }

          if (!transform(a, b)) {
            // drop path
            if (a==types::NONE || a==types::SPACE || a==types::NEWLINE) {
              if (b == types::SPACE || b == types::NEWLINE) {
                col = COL_NONE;
                collector.clear();
                a = b;
                return std::nullopt;
              }
              switch (b) {
                case (types::SPECIAL):
                  col = COL_MAX_CRUNCH;
                  break;
                case (types::CHAR):
                  col = COL_ALPHANUM;
                  break;
                case (types::NUM):
                  col = COL_LITERAL;
                  break;
                case (types::DQUOTE):
                  col = COL_DQUOTE;
                  break;
                case (types::QUOTE):
                  col = COL_QUOTE;
                  break;
                default: 
                  col = COL_NONE;
              }
              collector += symbol;
              a = b;
              return std::nullopt;
            }
            // drop handler
            //

            if (!collector.empty()) {
              switch (col) {
                case COL_MAX_CRUNCH:
                  max_munch();
                  break;

                case (COL_ALPHANUM):
                  insert_iden();
                  col = COL_NONE;
                  collector.clear();
                  a = types::NONE;
                  i--;
                  return std::nullopt;

                case (COL_LITERAL):
                  if (reg_lit()) {
                    col = COL_NONE;
                    a = types::NONE;
                    i--;
                    return std::nullopt;
                  }

                  col = COL_NONE;
                  collector.clear();
                  return gfl;
                default:
                  break;
              }
            }
          } else {
            collector += symbol;
          }
          escape_lifetime++;
          a = b; // swap with loss of b on next iteration
          return std::nullopt;
        };


        while (i < source.size()) {

          step_iterator(source[i]);

          if (gfl) return gfl;
          i++;
        }
        if (!collector.empty()) {
          switch (col) {
            case COL_MAX_CRUNCH:
              max_munch();
              break;

            case COL_ALPHANUM:
              insert_iden();
              break;

            case COL_LITERAL:
              if (!reg_lit())
                return gfl;
              break;

            default:
              break;
          }
        }
        file.parsed = true;
        return std::nullopt;
      }
  };
}
