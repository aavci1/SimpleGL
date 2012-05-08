#ifndef MESH_H
#define MESH_H

#include "Renderable.h"
#include "Types.h"

namespace SimpleGL {
  class MeshPrivate;

  class Mesh : public Renderable {
  public:
    Mesh(const String &name);
    ~Mesh();

    const String &name() const;

    const bool setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride);
    const bool setIndexData(uint indexData[], uint indexCount);

    void render(Camera *camera);

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
