#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include <string>

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
    Mesh *createSphere(float radius = 1.0f, float slices = 16.0f, float stacks = 16.0f);

    Mesh *loadMesh(const std::string &path);

  private:
    MeshManagerPrivate *d;
  };
}

#endif // MESHMANAGER_H
