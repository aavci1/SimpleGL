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

    BonePtr parent() const;
    void setParent(BonePtr parent);

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
