#ifndef SCENENODE_H
#define SCENENODE_H

#include "Types.h"

namespace SimpleGL {
  class SceneNodePrivate;
  class SceneObject;

  class SceneNode {
  public:
    SceneNode();
    ~SceneNode();

    SceneNode *parentSceneNode() const;
    void setParentSceneNode(SceneNode *node);

    SceneNode *createChildSceneNode(Vector3f position = Vector3f(0.0f, 0.0f, 0.0f), Quaternion orientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vector3f scale = Vector3f(1.0f, 1.0f, 1.0f));

    uint32_t numNodes() const;
    SceneNode *nodeAt(uint32_t index) const;
    void attachNode(SceneNode *node);

    uint32_t numObjects() const;
    SceneObject *objectAt(uint32_t index) const;
    void attachObject(SceneObject *object);

    const Vector3f &position() const;
    void setPosition(const Vector3f &position);
    void setPosition(float x, float y, float z);

    void moveRelative(const Vector3f &translation);
    void moveRelative(float x, float y, float z);

    const Quaternion &orientation() const;
    void setOrientation(const Quaternion &orientation);
    void setOrientation(float w, float x, float y, float z);

    void rotate(float angle, const Vector3f &axis, TransformSpace transformSpace = TS_LOCAL);

    void pitch(float angle, TransformSpace transformSpace = TS_LOCAL);
    void yaw(float angle, TransformSpace transformSpace = TS_LOCAL);
    void roll(float angle, TransformSpace transformSpace = TS_LOCAL);

    const Vector3f &scale() const;
    void setScale(const Vector3f &scale);
    void setScale(float x, float y, float z);

    const Matrix4f &transform() const;

    const Vector3f &worldPosition() const;
    const Quaternion &worldOrientation() const;
    const Vector3f &worldScale() const;

    const Matrix4f &worldTransform() const;

    void calculateWorldTransform();

  private:
    SceneNodePrivate *d;
  };

}
#endif // SCENENODE_H
