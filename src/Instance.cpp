#include "Instance.h"

#include "Material.h"
#include "Mesh.h"
#include "Program.h"
#include "Root.h"
#include "Texture.h"

namespace SimpleGL {
  class InstancePrivate {
  public:
    InstancePrivate(const String &name) : name(name), mesh("") {
    }

    ~InstancePrivate() {
    }

    String name { "" };
    String mesh { "" };
  };

  Instance::Instance(const String &name) : d(new InstancePrivate(name)) {
  }

  Instance::~Instance() {
    delete d;
  }

  const String &Instance::name() const {
    return d->name;
  }

  const String &Instance::mesh() const {
    return d->mesh;
  }

  void Instance::setMesh(const String &mesh) {
    d->mesh = mesh;
  }

  void Instance::render(Camera *camera) {
    Mesh *mesh = Root::instance()->retrieveMesh(d->mesh);
    if (mesh)
      mesh->render(camera);
  }
}
