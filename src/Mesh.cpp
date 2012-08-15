#include "Mesh.h"

#include "SubMesh.h"

#include <vector>

namespace SimpleGL {
  class MeshPrivate {
  public:
    MeshPrivate() {
    }

    ~MeshPrivate() {
      for (uint i = 0; i < subMeshes.size(); ++i)
        delete subMeshes[i];
    }

    String name;
    std::vector<SubMesh *> subMeshes;
  };

  Mesh::Mesh(const String &name) : d(new MeshPrivate()){
    d->name = name;
  }

  Mesh::~Mesh() {
    delete d;
  }

  const String &Mesh::name() const {
    return d->name;
  }

  uint32_t Mesh::numSubMeshes() const {
    return d->subMeshes.size();
  }

  SubMesh *Mesh::subMeshAt(uint32_t index) const {
    return d->subMeshes.at(index);
  }

  SubMesh *Mesh::createSubMesh() {
    SubMesh *subMesh = new SubMesh();
    // add to list
    d->subMeshes.push_back(subMesh);
    // return mesh
    return subMesh;
  }

  void Mesh::render(Camera *camera) {
    for (uint i = 0; i < d->subMeshes.size(); ++i)
      d->subMeshes[i]->render(camera);
  }
}
