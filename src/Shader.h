#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <sys/types.h>

namespace SimpleGL {
  class Shader {
  public:
    Shader();
    ~Shader();

    virtual uint id() = 0;

    virtual bool compile() = 0;
    virtual std::string message() = 0;
  };
}

#endif // SHADER_H
