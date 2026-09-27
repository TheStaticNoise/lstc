#include <optional>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include "lexer.h"
// lstc c++ based compiler grows chaotically.
int main(void) {
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
  std::string source = "fn XD() -> none { return; }";
  std::vector<std::string> core_in_namespace = {"io", "exposed", "sys"};
  lstc::lexer::lex lex_obj;
  auto e = lex_obj.lex_string(source, "test", std::nullopt);
  if (e.value_or(0)) {
    std::cerr << "[lstc] lexer fault! gfl set to {" << e.value() << "} [exiting...]\n";
    std::abort();
  }
  auto fid = lex_obj.find_file("test");
  if (!fid.has_value()) {return 1;}
  lstc::lexer::file_data file = lex_obj.get_file(fid.value());
  std::cout << "tok count: " << file.tokens.size() << "\nparsed: " << file.parsed << std::endl; 
  for (uint64_t i = 0; i < file.tokens.size(); i++) {
    resolve_tok(file.tokens[i]);
  } 
  std::cout << std::endl;
  
  return 0;
}
