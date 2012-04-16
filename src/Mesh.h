#ifndef SUBMESH_H
#define SUBMESH_H

#include <sys/types.h>

#include <string>

namespace SimpleGL {
  class MeshPrivate;

  class Mesh {
  public:
    Mesh();
    ~Mesh();

    bool setVertexData(uint vertexFormat, float vertexData[], uint vertexCount, uint stride);
    bool setIndexData(uint indexData[], uint indexCount);

    void setMaterialName(const std::string &name);
    const std::string &materialName() const;

    bool render() const;

  private:
    MeshPrivate *d;
  };
  
}

#endif // SUBMESH_H
