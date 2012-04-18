#include "Mesh.h"

#include "AxisAlignedBoundingBox.h"
#include "SubMesh.h"

namespace SimpleGL {
  class MeshPrivate {
  public:
    MeshPrivate() {
    }

    ~MeshPrivate() {
      for (int i = 0; i < subMeshes.size(); ++i)
        delete subMeshes[i];
    }

    AxisAlignedBoundingBox aabb;
    std::vector<SubMesh *> subMeshes;
  };

  Mesh::Mesh() : d(new MeshPrivate()) {
  }

  Mesh::~Mesh() {
    delete d;
  }

  AxisAlignedBoundingBox &Mesh::aabb() const {
    return d->aabb;
  }

  SubMesh *Mesh::createSubMesh() {
    SubMesh *subMesh = new SubMesh();
    // append to the list
    d->subMeshes.push_back(subMesh);
    // return subMesh
    return subMesh;
  }

  const std::vector<SubMesh *> &Mesh::subMeshes() const {
    return d->subMeshes;
  }

  void Mesh::setMaterialName(const std::string &materialName) {
    for (int i = 0; i < d->subMeshes.size(); ++i)
      d->subMeshes[i]->setMaterialName(materialName);
  }
}
