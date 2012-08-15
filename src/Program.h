#ifndef ROGRAM_H
#define ROGRAM_H

#include "Types.h"

namespace SimpleGL {
  class ProgramPrivate;
  class Shader;

  class Program {
  public:
    Program(const string &name);
    ~Program();

    const string &name() const;

    const bool loadShaderFromPath(ShaderType type, const string &path);
    const bool loadShaderFromSource(ShaderType type, const string &source);

    const bool link();

    const string &log() const;

    const bool setUniform(const string &name, uint value) const;
    const bool setUniform(const string &name, int value) const;
    const bool setUniform(const string &name, float value) const;
    const bool setUniform(const string &name, const Vector2f &value) const;
    const bool setUniform(const string &name, const Vector3f &value) const;
    const bool setUniform(const string &name, const Matrix4f &value) const;

    const bool bind() const;
    const bool unbind() const;

  private:
    ProgramPrivate *d;
  };
}
#endif // ROGRAM_H
