#ifndef MODEL_H
#define MODEL_H

#include "Types.h"

namespace SimpleGL {
  class ModelPrivate;

  class Model {
  public:
    Model(const string &name);
    ~Model();

    const string &name() const;

    const vector<MeshPtr> &meshes() const;
    MeshPtr createMesh();

    const vector<BonePtr> &bones() const;
    BonePtr createBone(const string &name);

    const vector<AnimationPtr> &animations() const;
    AnimationPtr createAnimation(const string &name);

    void updateAnimations(const long delta);

    void render(CameraPtr camera);

  private:
    ModelPrivate *d;
  };
}

#endif // MODEL_H
