#ifndef FRAGMENTSHADER_H
#define FRAGMENTSHADER_H

#include "Shader.h"

namespace SimpleGL {
  class FragmentShaderPrivate;

  class FragmentShader : public Shader {
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
