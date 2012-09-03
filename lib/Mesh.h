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

    const vector<shared_ptr<Animation>> &animations() const;
    shared_ptr<Animation> createAnimation(const string &name);

    const vector<shared_ptr<Bone>> &bones() const;
    shared_ptr<Bone> createBone(const string &name);

    const vector<shared_ptr<SubMesh>> &subMeshes() const;
    shared_ptr<SubMesh> createSubMesh();

    void updateBones();
    void render(shared_ptr<Camera> camera);

  private:
    MeshPrivate *d;
  };
}

#endif // MESH_H
