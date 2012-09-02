#ifndef MESH_H
#define MESH_H

#include "Types.h"

namespace SimpleGL {
  class Animation;
  class Bone;
  class Camera;
  class SubMesh;

  class MeshPrivate;

  class Mesh {
  public:
    Mesh(const string &name);
    ~Mesh();

    const string &name() const;

    const vector<Animation *> &animations() const;
    Animation *createAnimation(const string &name);

    const vector<Bone *> &bones() const;
    Bone *createBone(const string &name, Bone *parent);

    const vector<SubMesh *> &subMeshes() const;
    SubMesh *createSubMesh();

    void render(Camera *camera);

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
