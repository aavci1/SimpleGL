#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

namespace SimpleGL {
  class MeshPrivate;
  class AxisAlignedBoundingBox;
  class Camera;
  class SubMesh;

  class Mesh {
  public:
    Mesh();
    ~Mesh();

    AxisAlignedBoundingBox &aabb() const;

    SubMesh *createSubMesh();
    const std::vector<SubMesh *> &subMeshes() const;

    void setMaterialName(const std::string &materialName);

    const bool isVisibleFrom(Camera *camera) const;

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
