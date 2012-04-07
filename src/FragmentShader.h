#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

#include "AbstractShader.h"

namespace SimpleGL {
  class FragmentShaderPrivate;

  class FragmentShader : public AbstractShader {
  public:
    FragmentShader(std::string source);
    ~FragmentShader();

    virtual uint id();

    virtual bool compile();
    virtual std::string message();

  private:
    FragmentShaderPrivate *d;
  };
}

#endif // FRAGMENTSHADER_H
