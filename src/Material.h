#ifndef MATERIAL_H
#define MATERIAL_H

#include "Types.h"

namespace SimpleGL {
  class MaterialPrivate;
  class Program;
  class Texture;

  class Material {
  public:
    Material(const String &name);
    ~Material();

    const String &name() const;

    const String &program() const;
    void setProgram(const String &program);

    uint32_t numTextures() const;
    const String &textureAt(uint32_t index) const;
    void addTexture(const String &texture);

    void bind();
    void unbind();

  private:
    MaterialPrivate *d;
  };
}

#endif // MATERIAL_H
