#ifndef SUBMESH_H
#define SUBMESH_H

#include <sys/types.h>

#include <string>

namespace SimpleGL {
  class SubMeshPrivate;

  class SubMesh {
  public:
    SubMesh();
    ~SubMesh();

    bool setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride);
    bool setIndexData(uint indexData[], uint indexCount);

    void setMaterialName(const std::string &name);
    const std::string &materialName() const;

    bool render() const;

  private:
    SubMeshPrivate *d;
  };
}

#endif // SUBMESH_H
