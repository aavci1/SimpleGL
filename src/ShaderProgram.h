#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <sys/types.h>

#include <glm/glm.hpp>

namespace SimpleGL {
  class AbstractShader;
  class ShaderProgramPrivate;

  class ShaderProgram {
  public:
    ShaderProgram();
    ~ShaderProgram();

    void addShader(AbstractShader *shader);

    bool compileAndLink();

    std::string message();

    bool setUniform(std::string name, float value);
    bool setUniform(std::string name, glm::mat4 value);

    bool select() const;
    bool deselect() const;

  private:
    ShaderProgramPrivate *d;
  };
}
#endif // SHADERPROGRAM_H
