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

    const uint16_t vertexFormat() const;
    const uint16_t vertexSize() const;
    const uint32_t vertexCount() const;
    const char *vertexData() const;

    const uint16_t indexSize() const;
    const uint32_t indexCount() const;
    const char *indexData() const;

    const bool setVertexData(float *vertexData, uint vertexCount, uint vertexFormat);
    const bool setIndexData(uint32_t *indexData, uint indexCount);

    void render(Camera *camera);

  private:
    SubMeshPrivate *d;
  };
}

#endif // SUBMESH_H
