#ifndef MATERIAL_H
#define MATERIAL_H

#include "Types.h"

#include <vector>

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

    const std::vector<String> &textures() const;
    void addTexture(const String &texture);

    void bind();
    void unbind();

  private:
    MaterialPrivate *d;
  };
}

#endif // MATERIAL_H
