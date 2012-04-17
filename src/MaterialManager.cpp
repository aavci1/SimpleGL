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
    }

    std::map<std::string, Material *> materials;
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

  Material *MaterialManager::createMaterial(const std::string &materialName) {
    Material *material = new Material(materialName);
    // add material to the map
    d->materials[material->name()] = material;
    // return material
    return material;
  }

  Material *MaterialManager::getMaterialByName(const std::string &materialName) {
    return d->materials[materialName];
  }
}
