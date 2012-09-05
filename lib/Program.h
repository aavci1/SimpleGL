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

    void setUniform(const char *name, uint value) const;
    void setUniform(const char *name, int value) const;
    void setUniform(const char *name, float value) const;
    void setUniform(const char *name, const Vector2f &value) const;
    void setUniform(const char *name, const Vector3f &value) const;
    void setUniform(const char *name, const Matrix4f &value) const;
    void setUniform4fv(const char *name, const int numItems, const float *value) const;

    void bind() const;
  private:
    ProgramPrivate *d;
  };
}
#endif // ROGRAM_H
