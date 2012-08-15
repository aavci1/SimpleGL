#ifndef MATERIAL_H
#define MATERIAL_H

#include "Types.h"

namespace SimpleGL {
  class MaterialPrivate;
  class Program;
  class Texture;

  class Material {
  public:
    Material(const string &name);
    ~Material();

    const string &name() const;

    const string &program() const;
    void setProgram(const string &program);

    uint32_t numTextures() const;
    const string &textureAt(uint32_t index) const;
    void addTexture(const string &texture);

    void bind();
    void unbind();

  private:
    MaterialPrivate *d;
  };
}

#endif // MATERIAL_H
