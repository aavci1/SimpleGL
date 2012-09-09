#ifndef ROGRAM_H
#define ROGRAM_H

#include "Types.h"

namespace SimpleGL {
  class ProgramPrivate;

  class Program {
  public:
    Program(const string &name);
    ~Program();

    const string &name() const;

    const bool loadShaderFromPath(ShaderType type, const string &path);
    const bool loadShaderFromSource(ShaderType type, const string &source);

    const bool link();

    const string &log() const;

    void setUniform(const string &name, uint value) const;
    void setUniform(const string &name, int value) const;
    void setUniform(const string &name, float value) const;
    void setUniform(const string &name, const Vector2f &value) const;
    void setUniform(const string &name, const Vector3f &value) const;
    void setUniform(const string &name, const Matrix4f &value) const;
    void setUniform4fv(const string &name, const int numItems, const float *value) const;

    void bind() const;
  private:
    ProgramPrivate *d;
  };
}
#endif // ROGRAM_H
