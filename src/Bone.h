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

    const string &name() const;
    void setName(const string &name);

    const Matrix4f &offsetMatrix() const;
    void setOffsetMatrix(const Matrix4f &offsetMatrix);

    const Matrix4f &transform() const;
    void setTransform(const Matrix4f &transform);

    const Matrix4f &worldTransform() const;
    void calculateWorldTransform();

    vector<Bone *> &children() const;
    vector<SubMesh *> &subMeshes() const;

  private:
    BonePrivate *d;
  };
}

#endif // BONE_H
