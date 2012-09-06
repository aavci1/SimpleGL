#ifndef MESH_H
#define MESH_H

#include "Types.h"

namespace SimpleGL {
  class MeshPrivate;

  class Mesh {
  public:
    Mesh();
    ~Mesh();

    const string &material() const;
    void setMaterial(const string &name);

    const uint16_t vertexFormat() const;
    const uint16_t vertexSize() const;
    const uint32_t vertexCount() const;
    const char *vertexData() const;
    void setVertexData(float *vertexData, uint vertexCount, uint vertexFormat);

    const uint16_t indexSize() const;
    const uint32_t indexCount() const;
    const char *indexData() const;
    void setIndexData(uint32_t *indexData, uint indexCount);

    void render(CameraPtr camera);

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
