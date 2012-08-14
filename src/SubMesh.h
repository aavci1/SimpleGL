#ifndef SUBMESH_H
#define SUBMESH_H

#include "Renderable.h"
#include "Types.h"

namespace SimpleGL {
  class SubMeshPrivate;

  class SubMesh : public Renderable {
  public:
    SubMesh();
    ~SubMesh();

    const bool setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride);
    const bool setIndexData(uint indexData[], uint indexCount);

    void render(Camera *camera);

  private:
    SubMeshPrivate *d;
  };
}

#endif // SUBMESH_H
