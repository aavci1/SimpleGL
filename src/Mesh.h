#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

namespace SimpleGL {
  class MeshPrivate;
  class SubMesh;

  class Mesh {
  public:
    Mesh();
    ~Mesh();

    SubMesh *createSubMesh();
    const std::vector<SubMesh *> &subMeshes() const;

    void setMaterialName(const std::string &materialName);

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
