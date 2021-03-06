#include "Model.h"

#include "Animation.h"
#include "AnimationTrack.h"
#include "Bone.h"
#include "Mesh.h"

#include <queue>

namespace SimpleGL {
  class ModelPrivate {
  public:
    ModelPrivate() {
    }

    ~ModelPrivate() {
    }

    string name;
    vector<AnimationPtr> animations;
    vector<BonePtr> bones;
    vector<MeshPtr> meshes;
    long animationTime { 0 };
    float *transforms { nullptr };
  };

  Model::Model(const string &name) : d(new ModelPrivate()) {
    d->name = name;
  }

  Model::~Model() {
    delete d;
  }

  const string &Model::name() const {
    return d->name;
  }

  const vector<AnimationPtr> &Model::animations() const {
    return d->animations;
  }

  AnimationPtr Model::createAnimation(const string &name) {
    AnimationPtr animation { new Animation(name) };
    // add to list
    d->animations.push_back(animation);
    // return animation
    return animation;
  }

  void Model::updateAnimations(const long delta) {
    // increase animation time
    d->animationTime += delta;
    // update each animation
    for (AnimationPtr animation: d->animations) {
      // update bone transforms
      for (AnimationTrackPtr track: animation->tracks())
        for (BonePtr bone: d->bones)
          if (bone->name() == track->name())
            bone->setTransform(track->transform(d->animationTime % animation->duration()));
    }
    // update skeleton
    if (d->bones.size() == 0)
      return;
    // process bone
    d->bones.at(0)->updateWorldTransform();
    // update transforms
    if (!d->transforms)
      d->transforms = new float[d->bones.size() * 16];
    for (uint i = 0; i < d->bones.size(); ++i) {
      Matrix4f transform = d->bones.at(i)->worldTransform() * d->bones.at(i)->offsetMatrix();
      const float *values = glm::value_ptr(transform);
      for (uint j = 0; j < 16; ++j)
        d->transforms[i * 16 + j] = values[j];
    }
  }

  const vector<BonePtr> &Model::bones() const {
    return d->bones;
  }

  BonePtr Model::createBone(const string &name) {
    BonePtr bone { new Bone(name) };
    // add to list
    d->bones.push_back(bone);
    // reset transforms array
    delete d->transforms;
    d->transforms = nullptr;
    // return bone
    return bone;
  }

  const float *Model::boneTransforms() const {
    return d->transforms;
  }

  const vector<MeshPtr> &Model::meshes() const {
    return d->meshes;
  }

  MeshPtr Model::createMesh() {
    MeshPtr mesh { new Mesh() };
    // add to list
    d->meshes.push_back(mesh);
    // return mesh
    return mesh;
  }

  void Model::render(CameraPtr camera) {
    for (uint i = 0; i < d->meshes.size(); ++i)
      d->meshes[i]->render(camera);
  }
}
