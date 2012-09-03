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
    shared_ptr<Bone> parent { nullptr };
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

  shared_ptr<Bone> Bone::parent() const {
    return d->parent;
  }

  void Bone::setParent(shared_ptr<Bone> parent) {
    d->parent = parent;
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
  }
}
