#include "Instance.h"

namespace SimpleGL {
  class InstancePrivate {
  public:
    InstancePrivate() {
    }

    ~InstancePrivate() {
    }

    string mesh { "" };
    string material { "" };
  };

  Instance::Instance(const string &mesh, const string &material) : d(new InstancePrivate()) {
    d->mesh = mesh;
    d->material = material;
  }

  Instance::~Instance() {
    delete d;
  }

  const string &Instance::mesh() const {
    return d->mesh;
  }

  void Instance::setMesh(const string &mesh) {
    d->mesh = mesh;
  }

  const string &Instance::material() const {
    return d->material;
  }

  void Instance::setMaterial(const string &material) {
    d->material = material;
  }
}
