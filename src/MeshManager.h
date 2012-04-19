#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <string>
#include <sys/types.h>

namespace SimpleGL {
  class MeshManagerPrivate;
  class Mesh;

  class MeshManager {
  public:
    MeshManager();
    ~MeshManager();

    static MeshManager *instance();

    Mesh *createQuad(float width = 1.0f, float height = 1.0f);
    Mesh *createPlane(float width = 1.0f, float height = 1.0f, float uTile = 1.0f, float vTile = 1.0f);
    Mesh *createCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    Mesh *createSphere(float radius = 1.0f, uint slices = 16, uint stacks = 16);

    Mesh *loadMesh(const std::string &path);

  private:
    MeshManagerPrivate *d;
  };
}

#endif // MESHMANAGER_H
