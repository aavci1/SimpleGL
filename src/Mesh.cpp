#include "Mesh.h"

#include "Animation.h"
#include "Bone.h"
#include "SubMesh.h"

namespace SimpleGL {
  class MeshPrivate {
  public:
    MeshPrivate() {
    }

    ~MeshPrivate() {
      for (uint i = 0; i < animations.size(); ++i)
        delete animations[i];
      for (uint i = 0; i < bones.size(); ++i)
        delete bones[i];
      for (uint i = 0; i < subMeshes.size(); ++i)
        delete subMeshes[i];
    }

    string name;
    vector<Animation *> animations;
    vector<Bone *> bones;
    vector<SubMesh *> subMeshes;
  };

  Mesh::Mesh(const string &name) : d(new MeshPrivate()){
    d->name = name;
    d->bones.push_back(new Bone());
  }

  Mesh::~Mesh() {
    delete d;
  }

  const string &Mesh::name() const {
    return d->name;
  }

  vector<Animation *> &Mesh::animations() const {
    return d->animations;
  }

  Animation *Mesh::createAnimation(const string &name) {
    Animation *animation = new Animation(name);
    // add to list
    d->animations.push_back(animation);
    // return animation
    return animation;
  }

  vector<Bone *> &Mesh::bones() const {
    return d->bones;
  }

  uint32_t Mesh::numBones() const {
    return d->bones.size();
  }

  Bone *Mesh::boneAt(uint32_t index) const {
    return d->bones.at(index);
  }

  Bone *Mesh::createBone(Bone *parent) {
    Bone *bone = new Bone(parent);
    // add to list
    d->bones.push_back(bone);
    // return bone
    return bone;
  }

  uint32_t Mesh::numSubMeshes() const {
    return d->subMeshes.size();
  }

  SubMesh *Mesh::subMeshAt(uint32_t index) const {
    return d->subMeshes.at(index);
  }

  SubMesh *Mesh::createSubMesh(Bone *parent) {
    SubMesh *subMesh = new SubMesh();
    // add to list
    d->subMeshes.push_back(subMesh);
    // attach to bone
    if (parent)
      parent->subMeshes().push_back(subMesh);
    else
      d->bones.at(0)->subMeshes().push_back(subMesh);
    // return mesh
    return subMesh;
  }

  void Mesh::render(Camera *camera) {
    for (uint i = 0; i < d->subMeshes.size(); ++i)
      d->subMeshes[i]->render(camera);
  }
}
