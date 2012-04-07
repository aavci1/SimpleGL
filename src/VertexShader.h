#ifndef VERTEXSHADER_H
#define VERTEXSHADER_H

#include "AbstractShader.h"

namespace SimpleGL {
  class VertexShaderPrivate;

  class VertexShader : public AbstractShader {
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
