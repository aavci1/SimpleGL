#include "Bone.h"

namespace SimpleGL {
  class BonePrivate {
  public:
    BonePrivate() {
    }

    ~BonePrivate() {
    }

    string name { "" };

    Bone *parent { nullptr };
    vector<BonePtr> bones;

    Matrix4f offsetMatrix;
    Matrix4f transform;
    Matrix4f worldTransform;
  };

  Bone::Bone(const string &name) : d(new BonePrivate()) {
    d->name = name;
  }

  Bone::~Bone() {
    delete d;
  }

  const string &Bone::name() const {
    return d->name;
  }

  Bone *Bone::parent() const {
    return d->parent;
  }

  void Bone::attach(BonePtr bone) {
    d->bones.push_back(bone);
    bone->d->parent = this;
  }

  const vector<BonePtr> &Bone::bones() const {
    return d->bones;
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

  void Bone::updateWorldTransform(const Matrix4f &parentTransform) {
    d->worldTransform = parentTransform * d->transform;
    // update attached bones
    for (BonePtr bone: d->bones)
      bone->updateWorldTransform(d->worldTransform);
  }
}
