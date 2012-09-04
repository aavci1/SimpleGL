#include "SceneNode.h"

#include "Root.h"
#include "SceneObject.h"

namespace SimpleGL {
  class SceneNodePrivate {
  public:
    SceneNodePrivate() {
    }

    ~SceneNodePrivate() {
    }

    vector<SceneNodePtr> attachedNodes;
    vector<SceneObjectPtr> attachedObjects;

    Vector3f position { 0.0f, 0.0f, 0.0f };
    Quaternion orientation { 1.0f, 0.0f, 0.0f, 0.0f };
    Vector3f scale { 1.0f, 1.0f, 1.0f };

    bool recalcTransform { true };
    Matrix4f transform;

    Vector3f worldPosition { 0.0f, 0.0f, 0.0f };
    Quaternion worldOrientation { 1.0f, 0.0f, 0.0f, 0.0f };
    Vector3f worldScale { 1.0f, 1.0f, 1.0f };
    Matrix4f worldTransform;
  };

  SceneNode::SceneNode() : d(new SceneNodePrivate()) {
  }

  SceneNode::~SceneNode() {
    delete d;
  }

  void SceneNode::attachNode(SceneNodePtr node) {
    d->attachedNodes.push_back(node);
    node->setParent(this);
  }

  const vector<SceneNodePtr> &SceneNode::attachedNodes() const {
    return d->attachedNodes;
  }

  void SceneNode::attachObject(SceneObjectPtr object) {
    d->attachedObjects.push_back(object);
    object->setParent(this);
  }

  const vector<SceneObjectPtr> &SceneNode::attachedObjects() const {
    return d->attachedObjects;
  }

  string SceneNode::type() const {
    return "SceneNode";
  }

  const Vector3f &SceneNode::position() const {
    return d->position;
  }

  void SceneNode::setPosition(const Vector3f &position) {
    d->position = position;
    // recalculate transform
    d->recalcTransform = true;
  }

  void SceneNode::setPosition(float x, float y, float z) {
    setPosition(Vector3f(x, y, z));
  }

  void SceneNode::moveRelative(const Vector3f &translation) {
    d->position += worldOrientation() * translation;
    // recalculate transform
    d->recalcTransform = true;
  }

  void SceneNode::moveRelative(float x, float y, float z) {
    moveRelative(Vector3f(x, y, z));
  }

  const Quaternion &SceneNode::orientation() const {
    return d->orientation;
  }

  void SceneNode::setOrientation(const Quaternion &orientation) {
    d->orientation = glm::normalize(orientation);
    // recalculate transform
    d->recalcTransform = true;
  }

  void SceneNode::setOrientation(float w, float x, float y, float z) {
    setOrientation(Quaternion(w, x, y, z));
  }

  void SceneNode::rotate(float angle, const Vector3f &axis, TransformSpace transformSpace) {
    if (transformSpace == TS_WORLD)
      setOrientation(glm::angleAxis(angle, axis) * d->orientation);
    else if (transformSpace == TS_LOCAL)
      setOrientation(d->orientation * glm::angleAxis(angle, axis));
  }

  void SceneNode::pitch(float angle, TransformSpace transformSpace) {
    rotate(angle, Vector3f(1.0f, 0.0f, 0.0f), transformSpace);
  }

  void SceneNode::yaw(float angle, TransformSpace transformSpace) {
    rotate(angle, Vector3f(0.0f, 1.0f, 0.0f), transformSpace);
  }

  void SceneNode::roll(float angle, TransformSpace transformSpace) {
    rotate(angle, Vector3f(0.0f, 0.0f, 1.0f), transformSpace);
  }

  const Vector3f &SceneNode::scale() const {
    return d->scale;
  }

  void SceneNode::setScale(const Vector3f &scale) {
    d->scale = scale;
    // recalculate transform
    d->recalcTransform = true;
  }

  void SceneNode::setScale(float x, float y, float z) {
    setScale(Vector3f(x, y, z));
  }

  const Matrix4f &SceneNode::transform() const {
    if (d->recalcTransform) {
      // calculate transform
      d->transform = glm::translate(d->position) * glm::mat4_cast(d->orientation) * glm::scale(d->scale);
      // reset flag
      d->recalcTransform = false;
    }
    // return transform
    return d->transform;
  }

  const Vector3f &SceneNode::worldPosition() const {
    return d->worldPosition;
  }

  const Quaternion &SceneNode::worldOrientation() const {
    return d->worldOrientation;
  }

  const Vector3f &SceneNode::worldScale() const {
    return d->worldScale;
  }

  const Matrix4f &SceneNode::worldTransform() const {
    return d->worldTransform;
  }

  void SceneNode::updateWorldTransform() {
    if (parent()) {
      d->worldOrientation = parent()->worldOrientation() * d->orientation;
      d->worldScale = parent()->worldScale() * d->scale;
      d->worldPosition = parent()->worldPosition() + parent()->worldOrientation() * parent()->worldScale() * d->position;
      d->worldTransform = glm::translate(d->worldPosition) * glm::mat4_cast(d->worldOrientation) * glm::scale(d->worldScale);
    } else {
      d->worldPosition = d->position;
      d->worldOrientation = d->orientation;
      d->worldScale = d->scale;
      d->worldTransform = transform();
    }
  }
}
