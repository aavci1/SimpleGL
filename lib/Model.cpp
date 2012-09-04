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
    // process bones
    queue<BonePtr> processQueue;
    // add root bone
    processQueue.push(d->bones.at(0));
    // process bones
    while (!processQueue.empty()) {
      BonePtr bone = processQueue.front();
      processQueue.pop();
      // process bone
      bone->updateWorldTransform();
      // add child bones for processing
      for (BonePtr childBone: d->bones)
        if (childBone->parent() == bone)
          processQueue.push(childBone);
    }
  }

  const vector<BonePtr> &Model::bones() const {
    return d->bones;
  }

  BonePtr Model::createBone(const string &name) {
    BonePtr bone { new Bone(name) };
    // add to list
    d->bones.push_back(bone);
    // return bone
    return bone;
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
