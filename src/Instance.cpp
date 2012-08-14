#include "Instance.h"

namespace SimpleGL {
  class InstancePrivate {
  public:
    InstancePrivate() {
    }

    ~InstancePrivate() {
    }

    String mesh { "" };
    String material { "" };
  };

  Instance::Instance(const String &mesh, const String &material) : d(new InstancePrivate()) {
    d->mesh = mesh;
    d->material = material;
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
