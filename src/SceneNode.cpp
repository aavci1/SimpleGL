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

    SceneNode *parentSceneNode { nullptr };

    std::vector<SceneNode *> attachedNodes;
    std::vector<SceneObject *> attachedObjects;

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

  SceneNode *SceneNode::parentSceneNode() const {
    return d->parentSceneNode;
  }

  void SceneNode::setParentSceneNode(SceneNode *node) {
    d->parentSceneNode = node;
  }

  SceneNode *SceneNode::createChildSceneNode(Vector3f position, Quaternion orientation, Vector3f scale) {
    SceneNode *sceneNode = Root::instance()->createSceneNode();
    // set parent
    attachNode(sceneNode);
    // set position/orientation/scale
    sceneNode->setPosition(position);
    sceneNode->setOrientation(orientation);
    sceneNode->setScale(scale);
    // return node
    return sceneNode;
  }

  void SceneNode::attachNode(SceneNode *node) {
    // add to the list
    d->attachedNodes.push_back(node);
    // set child nodes parent
    node->setParentSceneNode(this);
  }

  void SceneNode::attachObject(SceneObject *object) {
    // add to the list
    d->attachedObjects.push_back(object);
    // set child objects parent
    object->setParentSceneNode(this);
  }

  const std::vector<SceneNode *> &SceneNode::attachedNodes() const {
    return d->attachedNodes;
  }

  const std::vector<SceneObject *> &SceneNode::attachedObjects() const {
    return d->attachedObjects;
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

  void SceneNode::calculateWorldTransform() {
    if (d->parentSceneNode) {
      d->worldOrientation = d->parentSceneNode->worldOrientation() * d->orientation;
      d->worldScale = d->parentSceneNode->worldScale() * d->scale;
      d->worldPosition = d->parentSceneNode->worldPosition() + parentSceneNode()->worldOrientation() * parentSceneNode()->worldScale() * d->position;
      d->worldTransform = glm::translate(d->worldPosition) * glm::mat4_cast(d->worldOrientation) * glm::scale(d->worldScale);
    } else {
      d->worldPosition = d->position;
      d->worldOrientation = d->orientation;
      d->worldScale = d->scale;
      d->worldTransform = transform();
    }
  }
}
