#ifndef ROGRAM_H
#define ROGRAM_H

#include "Types.h"

#include <string>

namespace SimpleGL {
  class Shader;
  class ProgramPrivate;

  class Program {
  public:
    Program();
    ~Program();

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
    ProgramPrivate *d;
  };
}
#endif // ROGRAM_H
