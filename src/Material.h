#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

namespace SimpleGL {
  class MaterialPrivate;
  class Program;

  class Material {
  public:
    Material(const std::string &name);
    ~Material();

    const std::string &name() const;

    bool setProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
    Program *program() const;

    bool addTexture(const std::string &texturePath);

    void select();
    void deselect();

  private:
    MaterialPrivate *d;
  };
}

#endif // MATERIAL_H
