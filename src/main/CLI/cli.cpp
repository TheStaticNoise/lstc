#include <cstddef>
#include <string>
#include <iostream>
#include "../interface.h"


int main() {
    auto r = lstc::FileManag::path_dissect("~/HOME/$XD");
    lstc::FileManag::print_partSegs(r);
    auto re = lstc::FileManag::path_expand("~/XD/$VAREREEREENV", NULL);
    std::cout << std::endl;
    return 0;
}
