#include "Mesh.h"

#include "Animation.h"
#include "Bone.h"
#include "SubMesh.h"

#include <queue>

namespace SimpleGL {
  class MeshPrivate {
  public:
    MeshPrivate() {
    }

    ~MeshPrivate() {
    }

    string name;
    vector<AnimationPtr> animations;
    vector<BonePtr> bones;
    vector<SubMeshPtr> subMeshes;
  };

  Mesh::Mesh(const string &name) : d(new MeshPrivate()){
    d->name = name;
  }

  Mesh::~Mesh() {
    delete d;
  }

  const string &Mesh::name() const {
    return d->name;
  }

  const vector<AnimationPtr> &Mesh::animations() const {
    return d->animations;
  }

  AnimationPtr Mesh::createAnimation(const string &name) {
    AnimationPtr animation { new Animation(name) };
    // add to list
    d->animations.push_back(animation);
    // return animation
    return animation;
  }

  const vector<BonePtr> &Mesh::bones() const {
    return d->bones;
  }

  BonePtr Mesh::createBone(const string &name) {
    BonePtr bone { new Bone(name) };
    // add to list
    d->bones.push_back(bone);
    // return bone
    return bone;
  }

  const vector<SubMeshPtr> &Mesh::subMeshes() const {
    return d->subMeshes;
  }

  SubMeshPtr Mesh::createSubMesh() {
    SubMeshPtr subMesh { new SubMesh() };
    // add to list
    d->subMeshes.push_back(subMesh);
    // return mesh
    return subMesh;
  }

  void Mesh::updateBones() {
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

  void Mesh::render(CameraPtr camera) {
    for (uint i = 0; i < d->subMeshes.size(); ++i)
      d->subMeshes[i]->render(camera);
  }
}
