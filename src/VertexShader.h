#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "Shader.h"

namespace SimpleGL {
  class VertexShaderPrivate;

  class VertexShader : public Shader {
  public:
    VertexShader(std::string source);
    ~VertexShader();

    virtual uint id();

    virtual bool compile();
    virtual std::string message();

  private:
    VertexShaderPrivate *d;
  };
}

#endif // VERTEXSHADER_H
