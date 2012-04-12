#ifndef SUBMESH_H
#define SUBMESH_H

#include <sys/types.h>

namespace SimpleGL {
  class SubMeshPrivate;

  class Mesh {
  public:
    Mesh();
    ~Mesh();

    bool setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride);
    bool setIndexData(uint indexData[], uint indexCount);

    bool render() const;

  private:
    SubMeshPrivate *d;
  };
  
}

#endif // SUBMESH_H
