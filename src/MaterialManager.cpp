#include "MaterialManager.h"

#include "Material.h"

#include <map>

namespace SimpleGL {
  static MaterialManager *_instance = 0;

  class MaterialManagerPrivate {
  public:
    MaterialManagerPrivate() {
    }

    ~MaterialManagerPrivate() {
      std::map<std::string, Material*>::iterator it = materials.begin();
      for (it = materials.begin(); it != materials.end(); ++it)
        delete it->second;
      // delete light materials
      std::map<LightType, Material*>::iterator it2 = lightMaterials.begin();
      for (it2 = lightMaterials.begin(); it2 != lightMaterials.end(); ++it2)
        delete it2->second;
    }

    std::map<std::string, Material *> materials;
    std::map<LightType, Material *> lightMaterials;
  };

  MaterialManager::MaterialManager() : d(new MaterialManagerPrivate()) {
  }

  MaterialManager::~MaterialManager() {
    delete d;
  }

  MaterialManager *MaterialManager::instance() {
    if (!_instance)
      _instance = new MaterialManager();
    // return instance
    return _instance;
  }

  Material *MaterialManager::getMaterialByName(const std::string &materialName) {
    // create a new material, if none exists
    if (!d->materials[materialName])
      d->materials[materialName] = new Material(materialName);
    // return material
    return d->materials[materialName];
  }

  Material *MaterialManager::getMaterialByLightType(const LightType type) {
    // create a new material, if none exists
    if (!d->lightMaterials[type])
      d->lightMaterials[type] = new Material("");
    // return material
    return d->lightMaterials[type];
  }
}
