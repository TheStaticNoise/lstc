#include <cstddef>
#include <cstdio>
#include <optional>
#include <pthread.h>
#include <sys/types.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "lexer.h"
#include "simplify.h"
#include <fcntl.h>
#include <unistd.h>
#include <print>
// lstc c++ based compiler grows chaotically.
//
//

bool readfd(int fd, char* buffer, size_t n) {
  for (uint64_t i = 0; i < n;) {
    auto d = read(fd, buffer + i, n-i);
    if (d <= 0) return false;
    i+=d;
  }
  return true;
}

int main(int argc, char** argv) {
  if (argc == 1) {
    std::println("needed 2 args", 2);
    return -1;
  }
  auto resolve_tok = [&](lstc::lexer::token_value tok) {
    if (tok >= lstc::lexer::token_value::IDENTITY_BASE) {
      std::cout << "[tok identity] ";
      return;
    }
    auto e = lstc::lexer::tokens_to_string.find(tok);
    if (e != lstc::lexer::tokens_to_string.end()) {
      std::cout << "[tok " << e->second << "] ";
      return;
    } 
    std::cout << "[tok unk] ";
    return;
  };
  
  // simple open mechanism
  
  int fd = open(argv[1], O_RDONLY);
  off_t len = lseek(fd, 0, SEEK_END);
  if (len <= 0) {
    std::println("Err: empty file", 2);
    return -2;
  }
  lseek(fd, 0, SEEK_SET);
  char* buffer = new char[len+1];
  readfd(fd, buffer, len);
  buffer[len] = '\0';

  // to string

  std::string source = std::string(buffer);
  
  std::vector<std::string> core_in_namespace = {"io", "exposed", "sys"};
  lstc::lexer::lex lex_obj;
  std::string name = "test";

  auto e = lex_obj.lex_string(source, name, std::nullopt);
  if (e.value_or(0)) {
    std::cerr << "[lstc] lexer fault! gfl set to {" << e.value() << "} [exiting...]\n";
    std::abort();
  }
  auto fid = lex_obj.find_file(name);
  if (!fid.has_value()) {return -3;}
  lstc::lexer::file_data file = lex_obj.get_file(fid.value());
  std::cout << "tok count: " << file.tokens.size() << "\nparsed: " << file.parsed << std::endl; 
  for (uint64_t i = 0; i < file.tokens.size(); i++) {
    resolve_tok(file.tokens[i]);
  } 
  std::cout << std::endl;
  lstc::simplify::object simplify;
  simplify.set_lexerInstance(lex_obj);
  uint64_t id;
  uint64_t a = simplify.loadfile(name, &id);
  if (a) {
    std::println("Simplify error {}", a, 2);
    return -4;
  }  uint64_t err;
  auto dirs = simplify.dirAnalyze(id, &err);
  if (!dirs.has_value()) {
    std::println("Dir Analyze error {}", err, 2);
  }
  if (dirs.has_value()) {
  for (auto dir : dirs.value()) {
    switch (dir.type) {
      case (lstc::simplify::object::directive::DIR_TYPE::INCLUDE):
        std::print("[INCLUDE ");
        break;
      case (lstc::simplify::object::directive::DIR_TYPE::ENTRY):
        std::print("[ENTRY ");
        break;
      default:
        std::print("UNKNOWN VALUE TYPE");
        std::abort();
    }
    std::println("identity id: {}]", dir.identity_id);
  }
  }
  auto identity = lex_obj.get_identity(2);
  std::println("\n{}", std::get<std::string>(identity.data), 2);
  return 0;
}
