#ifndef ROGRAM_H
#define ROGRAM_H

#include "Types.h"

namespace SimpleGL {
  class ProgramPrivate;
  class Shader;

  class Program {
  public:
    Program(const String &name);
    ~Program();

    const String &name() const;

    const bool loadShaderFromPath(ShaderType type, const String &path);
    const bool loadShaderFromSource(ShaderType type, const String &source);

    const bool link();

    const String &errorMessage() const;

    const bool setUniform(const String &name, uint value) const;
    const bool setUniform(const String &name, int value) const;
    const bool setUniform(const String &name, float value) const;
    const bool setUniform(const String &name, const Vector2f &value) const;
    const bool setUniform(const String &name, const Vector3f &value) const;
    const bool setUniform(const String &name, const Matrix4f &value) const;

    const bool bind() const;
    const bool unbind() const;

  private:
    ProgramPrivate *d;
  };
}
#endif // ROGRAM_H
