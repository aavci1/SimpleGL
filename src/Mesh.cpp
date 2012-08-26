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
    d->bones.push_back(new Bone("Root"));
  }

  Mesh::~Mesh() {
    delete d;
  }

  const string &Mesh::name() const {
    return d->name;
  }

  const vector<Animation *> &Mesh::animations() const {
    return d->animations;
  }

  Animation *Mesh::createAnimation(const string &name) {
    Animation *animation = new Animation(name);
    // add to list
    d->animations.push_back(animation);
    // return animation
    return animation;
  }

  const vector<Bone *> &Mesh::bones() const {
    return d->bones;
  }

  Bone *Mesh::createBone(const string &name, Bone *parent) {
    Bone *bone = new Bone(name, parent);
    // add to list
    d->bones.push_back(bone);
    // return bone
    return bone;
  }

  const vector<SubMesh *> &Mesh::subMeshes() const {
    return d->subMeshes;
  }

  SubMesh *Mesh::createSubMesh() {
    SubMesh *subMesh = new SubMesh();
    // add to list
    d->subMeshes.push_back(subMesh);
    // return mesh
    return subMesh;
  }

  void Mesh::render(Camera *camera) {
    for (uint i = 0; i < d->subMeshes.size(); ++i)
      d->subMeshes[i]->render(camera);
  }
}
