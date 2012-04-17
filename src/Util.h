#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace SimpleGL {
  class Util {
  private:
    Util();

  public:
    static std::string readAll(const std::string &path);
  };
}
#endif // UTIL_H
