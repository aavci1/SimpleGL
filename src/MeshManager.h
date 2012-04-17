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

    Mesh *loadMesh(const std::string &path);

  private:
    MeshManagerPrivate *d;
  };
}

#endif // MESHMANAGER_H
