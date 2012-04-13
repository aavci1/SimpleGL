#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <sys/types.h>

#include <glm/glm.hpp>

namespace SimpleGL {
  class Shader;
  class ShaderProgramPrivate;

  class ShaderProgram {
  public:
    ShaderProgram();
    ~ShaderProgram();

    void addShader(Shader *shader);

    bool compileAndLink();

    std::string message();

    bool setUniform(std::string name, uint value);
    bool setUniform(std::string name, int value);
    bool setUniform(std::string name, float value);
    bool setUniform(std::string name, const glm::vec2 &value);
    bool setUniform(std::string name, const glm::vec3 &value);
    bool setUniform(std::string name, const glm::mat4 &value);

    bool select() const;
    bool deselect() const;

  private:
    ShaderProgramPrivate *d;
  };
}
#endif // SHADERPROGRAM_H
