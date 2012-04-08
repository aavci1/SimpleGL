#ifndef SUBMESH_H
#define SUBMESH_H

#include <sys/types.h>

namespace SimpleGL {
  class SubMeshPrivate;

  class SubMesh {
  public:
    SubMesh();
    ~SubMesh();

    const uint id() const;

    bool setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride);
    bool setIndexData(uint indexData[], uint indexCount);

    bool draw() const;

  private:
    SubMeshPrivate *d;
  };
  
}

#endif // SUBMESH_H
