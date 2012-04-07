#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <sys/types.h>

namespace SimpleGL {
  class AbstractShader;
  class ShaderProgramPrivate;

  class ShaderProgram {
  public:
    ShaderProgram();
    ~ShaderProgram();

    uint id();

    void addShader(AbstractShader *shader);
    bool compileAndLink();

    std::string message();

    bool setUniform(std::string name, float value);

  private:
    ShaderProgramPrivate *d;
  };
}
#endif // SHADERPROGRAM_H
