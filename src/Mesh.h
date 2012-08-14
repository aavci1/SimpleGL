#ifndef MESH_H
#define MESH_H

#include "Types.h"

namespace SimpleGL {
  class Camera;
  class SubMesh;

  class MeshPrivate;

  class Mesh {
  public:
    Mesh(const String &name);
    ~Mesh();

    const String &name() const;

    uint32_t numSubMeshes() const;
    SubMesh *subMeshAt(uint32_t index) const;
    SubMesh *createSubMesh();

    void render(Camera *camera);

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
