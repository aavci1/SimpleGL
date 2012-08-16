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
    vector<Bone *> children;
    vector<SubMesh *> meshes;
  };

  Bone::Bone(Bone *parent) : d(new BonePrivate()) {
    d->parent = parent;
  }

  Bone::~Bone() {
    delete d;
  }

  const string &Bone::name() const {
    return d->name;
  }

  void Bone::setName(const string &name) {
    d->name = name;
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
    for (Bone *bone: d->children)
      bone->updateWorldTransform();
  }

  vector<Bone *> &Bone::children() const {
    return d->children;
  }

  vector<SubMesh *> &Bone::subMeshes() const {
    return d->meshes;
  }
}
