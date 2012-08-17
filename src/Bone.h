#ifndef BONE_H
#define BONE_H

#include "Types.h"

namespace SimpleGL {
  class SubMesh;

  class BonePrivate;

  class Bone {
  public:
    Bone(Bone *parent = nullptr);
    ~Bone();

    Bone *parentBone() const;
    void setParentBone(Bone *parent);

    const string &name() const;
    void setName(const string &name);

    const Matrix4f &offsetMatrix() const;
    void setOffsetMatrix(const Matrix4f &offsetMatrix);

    const Matrix4f &transform() const;
    void setTransform(const Matrix4f &transform);

    const Matrix4f &worldTransform() const;
    void updateWorldTransform();

    const vector<Bone *> &childBones() const;
    void attachBone(Bone *bone);

  private:
    BonePrivate *d;
  };
}

#endif // BONE_H
