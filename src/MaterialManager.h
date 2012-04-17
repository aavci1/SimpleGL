#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <string>

namespace SimpleGL {
  class MaterialManagerPrivate;
  class Material;

  class MaterialManager {
  public:
    MaterialManager();
    ~MaterialManager();

    static MaterialManager *instance();

    Material *createMaterial(const std::string &materialName);

    Material *getMaterialByName(const std::string &materialName);

  private:
    MaterialManagerPrivate *d;
  };
}

#endif // MATERIALMANAGER_H
