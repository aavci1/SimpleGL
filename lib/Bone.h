#ifndef BONE_H
#define BONE_H

#include "Types.h"

namespace SimpleGL {
  class BonePrivate;

  class Bone {
  public:
    Bone(const string &name);
    ~Bone();

    const string &name() const;

    Bone *parent() const;

    void attach(BonePtr bone);
    const vector<BonePtr> &bones() const;

    const Matrix4f &offsetMatrix() const;
    void setOffsetMatrix(const Matrix4f &offsetMatrix);

    const Matrix4f &transform() const;
    void setTransform(const Matrix4f &transform);

    const Matrix4f &worldTransform() const;
    void updateWorldTransform(const Matrix4f &parentTransform = Matrix4f());

  private:
    BonePrivate *d;
  };
}

#endif // BONE_H
