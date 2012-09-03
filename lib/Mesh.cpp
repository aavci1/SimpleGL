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
    vector<shared_ptr<Animation>> animations;
    vector<shared_ptr<Bone>> bones;
    vector<shared_ptr<SubMesh>> subMeshes;
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

  const vector<shared_ptr<Animation>> &Mesh::animations() const {
    return d->animations;
  }

  shared_ptr<Animation> Mesh::createAnimation(const string &name) {
    shared_ptr<Animation> animation { new Animation(name) };
    // add to list
    d->animations.push_back(animation);
    // return animation
    return animation;
  }

  const vector<shared_ptr<Bone>> &Mesh::bones() const {
    return d->bones;
  }

  shared_ptr<Bone> Mesh::createBone(const string &name) {
    shared_ptr<Bone> bone { new Bone(name) };
    // add to list
    d->bones.push_back(bone);
    // return bone
    return bone;
  }

  const vector<shared_ptr<SubMesh>> &Mesh::subMeshes() const {
    return d->subMeshes;
  }

  shared_ptr<SubMesh> Mesh::createSubMesh() {
    shared_ptr<SubMesh> subMesh { new SubMesh() };
    // add to list
    d->subMeshes.push_back(subMesh);
    // return mesh
    return subMesh;
  }

  void Mesh::updateBones() {
    if (d->bones.size() == 0)
      return;
    // process bones
    queue<shared_ptr<Bone>> processQueue;
    // add root bone
    processQueue.push(d->bones.at(0));
    // process bones
    while (!processQueue.empty()) {
      shared_ptr<Bone> bone = processQueue.front();
      processQueue.pop();
      // process bone
      bone->updateWorldTransform();
      // add child bones for processing
      for (shared_ptr<Bone> childBone: d->bones)
        if (childBone->parent() == bone)
          processQueue.push(childBone);
    }
  }

  void Mesh::render(shared_ptr<Camera> camera) {
    for (uint i = 0; i < d->subMeshes.size(); ++i)
      d->subMeshes[i]->render(camera);
  }
}
