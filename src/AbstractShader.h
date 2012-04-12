#ifndef ABSTRACTSHADER_H
#define ABSTRACTSHADER_H

#include <string>
#include <sys/types.h>

namespace SimpleGL {
  class AbstractShader {
  public:
    AbstractShader();
    ~AbstractShader();

    virtual uint id() = 0;

    virtual bool compile() = 0;
    virtual std::string message() = 0;
  };
}

#endif // ABSTRACTSHADER_H
