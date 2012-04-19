#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "Light.h"

#include <string>

namespace SimpleGL {
  class MaterialManagerPrivate;
  class Material;

  class MaterialManager {
  public:
    MaterialManager();
    ~MaterialManager();

    static MaterialManager *instance();

    Material *getMaterialByName(const std::string &materialName);
    Material *getMaterialByLightType(const LightType type);

  private:
    MaterialManagerPrivate *d;
  };
}

#endif // MATERIALMANAGER_H
