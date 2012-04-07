#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <sys/types.h>

namespace SimpleGL {
  enum ShaderAttributes {
    SGL_POSITION,   // 3 floats, "sglPosition"
    SGL_NORMAL,     // 3 floats, "sglNormal"
    SGL_COLOR,      // 3 floats, "sglColor"
    SGL_TEXCOORD0,  // 2 floats, "sglTexCoord0"
    SGL_TEXCOORD1,  // 2 floats, "sglTexCoord1"
    SGL_TEXCOORD2   // 2 floats, "sglTexCoord2"
  };

  class AbstractShader {
  public:
    AbstractShader();
    ~AbstractShader();

    virtual uint id() = 0;

    virtual bool compile() = 0;
    virtual std::string message() = 0;
  };
}

#endif // SHADER_H
