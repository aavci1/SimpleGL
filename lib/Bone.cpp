#include "Bone.h"

#include "SubMesh.h"

namespace SimpleGL {
  class BonePrivate {
  public:
    BonePrivate() {
    }

    ~BonePrivate() {
    }

    string name { "" };
    Bone *parent { nullptr };
    Matrix4f offsetMatrix;
    Matrix4f transform;
    Matrix4f worldTransform;
    vector<Bone *> childBones;
  };

  Bone::Bone(const string &name, Bone *parent) : d(new BonePrivate()) {
    d->name = name;
    d->parent = parent;
  }

  Bone::~Bone() {
    delete d;
  }

  const string &Bone::name() const {
    return d->name;
  }

  Bone *Bone::parentBone() const {
    return d->parent;
  }

  void Bone::setParentBone(Bone *parent) {
    d->parent = parent;
  }

  const vector<Bone *> &Bone::childBones() const {
    return d->childBones;
  }

  void Bone::attachBone(Bone *childBone) {
    // add to the list
    d->childBones.push_back(childBone);
    // set child nodes parent
    childBone->setParentBone(this);
  }

  const Matrix4f &Bone::offsetMatrix() const {
    return d->offsetMatrix;
  }

  void Bone::setOffsetMatrix(const Matrix4f &offsetMatrix) {
    d->offsetMatrix = offsetMatrix;
  }

  const Matrix4f &Bone::transform() const {
    return d->transform;
  }

  void Bone::setTransform(const Matrix4f &transform) {
    d->transform = transform;
  }

  const Matrix4f &Bone::worldTransform() const {
    return d->worldTransform;
  }

  void Bone::updateWorldTransform() {
    d->worldTransform = d->transform;
    if (d->parent)
      d->worldTransform = d->parent->worldTransform() * d->transform;
    for (Bone *bone: d->childBones)
      bone->updateWorldTransform();
  }
}
