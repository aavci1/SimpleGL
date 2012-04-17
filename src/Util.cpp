#include "Util.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace SimpleGL {
  Util::Util() {
  }

  std::string Util::readAll(const std::string &path) {
    std::ifstream in(path.c_str());
    std::stringstream buffer;
    buffer << in.rdbuf();

    return std::string(buffer.str());
  }
}
