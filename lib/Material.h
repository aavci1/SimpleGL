#ifndef MATERIAL_H
#define MATERIAL_H

#include "Types.h"

namespace SimpleGL {
  class MaterialPrivate;

  class Material {
  public:
    Material(const string &name);
    ~Material();

    const string &name() const;

    const string &program() const;
    void setProgram(const string &program);

    void addTexture(const string &path);

    void bind();
    void unbind();

  private:
    MaterialPrivate *d;
  };
}

#endif // MATERIAL_H
