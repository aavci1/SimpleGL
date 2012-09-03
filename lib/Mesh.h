#ifndef MESH_H
#define MESH_H

#include "Types.h"

namespace SimpleGL {
  class MeshPrivate;

  class Mesh {
  public:
    Mesh(const string &name);
    ~Mesh();

    const string &name() const;

    const vector<AnimationPtr> &animations() const;
    AnimationPtr createAnimation(const string &name);

    const vector<BonePtr> &bones() const;
    BonePtr createBone(const string &name);

    const vector<SubMeshPtr> &subMeshes() const;
    SubMeshPtr createSubMesh();

    void updateBones();
    void render(CameraPtr camera);

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
