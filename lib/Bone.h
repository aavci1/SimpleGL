#ifndef BONE_H
#define BONE_H

#include "Types.h"

namespace SimpleGL {
  class SubMesh;

  class BonePrivate;

  class Bone {
  public:
    Bone(const string &name, Bone *parent = nullptr);
    ~Bone();

    const string &name() const;

    Bone *parentBone() const;
    void setParentBone(Bone *parent);

    const vector<Bone *> &childBones() const;
    void attachBone(Bone *bone);

    const Matrix4f &offsetMatrix() const;
    void setOffsetMatrix(const Matrix4f &offsetMatrix);

    const Matrix4f &transform() const;
    void setTransform(const Matrix4f &transform);

    const Matrix4f &worldTransform() const;
    void updateWorldTransform();

  private:
    BonePrivate *d;
  };
}

#endif // BONE_H
