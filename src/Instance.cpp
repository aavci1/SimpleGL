#include "Instance.h"

#include "Material.h"
#include "Mesh.h"
#include "Program.h"
#include "Root.h"
#include "Texture.h"

namespace SimpleGL {
  class InstancePrivate {
  public:
    InstancePrivate(const String &mesh, const String &material) : mesh(mesh), material(material) {
    }

    ~InstancePrivate() {
    }

    String mesh { "" };
    String material { "" };
  };

  Instance::Instance(const String &mesh, const String &material) : d(new InstancePrivate(mesh, material)) {
  }

  Instance::~Instance() {
    delete d;
  }

  const String &Instance::mesh() const {
    return d->mesh;
  }

  void Instance::setMesh(const String &mesh) {
    d->mesh = mesh;
  }

  const String &Instance::material() const {
    return d->material;
  }

  void Instance::setMaterial(const String &material) {
    d->material = material;
  }
}
