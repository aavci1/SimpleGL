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

    vector<Animation *> &animations() const;
    Animation *createAnimation(const string &name);

    vector<Bone *> &bones() const;

    uint32_t numBones() const;
    Bone *boneAt(uint32_t index) const;
    Bone *createBone(Bone *parent);

    uint32_t numSubMeshes() const;
    SubMesh *subMeshAt(uint32_t index) const;
    SubMesh *createSubMesh(Bone *parent = nullptr);

    void render(Camera *camera);

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
