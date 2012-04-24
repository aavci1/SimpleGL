#include "SceneNode.h"

#include "Light.h"
#include "Mesh.h"
#include "Types.h"

namespace SimpleGL {
  class SceneNodePrivate {
  public:
    SceneNodePrivate() : position(0, 0, 0), orientation(1, 0, 0, 0), scale(1, 1, 1), recalcTransformationMatrix(false) {
    }

    ~SceneNodePrivate() {
    }

    std::vector<SceneNode *> nodes;
    std::vector<Mesh *> meshes;
    std::vector<Light *> lights;
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;
    bool recalcTransformationMatrix;
    glm::mat4 transformationMatrix;
  };

  SceneNode::SceneNode() : d(new SceneNodePrivate()) {
  }

  SceneNode::~SceneNode() {
    delete d;
  }

  SceneNode *SceneNode::createChildNode(const glm::vec3 &position, const glm::quat &orientation, const glm::vec3 &scale) {
    SceneNode *childNode = new SceneNode();
    // set position/orientation/scale
    childNode->setPosition(position);
    childNode->setOrientation(orientation);
    childNode->setScale(scale);
    // add to the list
    d->nodes.push_back(childNode);
    // return child node
    return childNode;
  }

  void SceneNode::attach(SceneNode *node) {
    d->nodes.push_back(node);
  }

  const std::vector<SceneNode *> &SceneNode::nodes() const {
    return d->nodes;
  }

  void SceneNode::attach(Mesh *mesh) {
    d->meshes.push_back(mesh);
  }

  const std::vector<Mesh *> &SceneNode::meshes() const {
    return d->meshes;
  }

  void SceneNode::attach(Light *light) {
    d->lights.push_back(light);
  }

  const std::vector<Light *> &SceneNode::lights() const {
    return d->lights;
  }

  void SceneNode::setPosition(const glm::vec3 &position) {
    d->position = position;
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void SceneNode::setPosition(float x, float y, float z) {
    d->position = glm::vec3(x, y, z);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::vec3 &SceneNode::position() const {
    return d->position;
  }

  void SceneNode::setOrientation(const glm::quat &orientation) {
    d->orientation = glm::normalize(orientation);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void SceneNode::setOrientation(float w, float x, float y, float z) {
    d->orientation = glm::normalize(glm::quat(w, x, y, z));
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::quat &SceneNode::orientation() const {
    return d->orientation;
  }

  void SceneNode::setScale(const glm::vec3 &scale) {
    d->scale = scale;
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void SceneNode::setScale(float x, float y, float z) {
    d->scale = glm::vec3(x, y, z);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::vec3 &SceneNode::scale() const {
    return d->scale;
  }

  void SceneNode::rotate(float angle, const glm::vec3 &axis, TransformSpace transformSpace) {
    if (transformSpace == TS_WORLD)
      setOrientation(glm::angleAxis(angle, axis) * d->orientation);
    else if (transformSpace == TS_LOCAL)
      setOrientation(d->orientation * glm::angleAxis(angle, axis));
  }

  void SceneNode::pitch(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f), transformSpace);
  }

  void SceneNode::yaw(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f), transformSpace);
  }

  void SceneNode::roll(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f), transformSpace);
  }

  const glm::mat4 &SceneNode::transformationMatrix() const {
    if (d->recalcTransformationMatrix) {
      glm::mat4 identity;
      // calculcate transformation matrix
      d->transformationMatrix = glm::translate(identity, d->position) * glm::mat4_cast(d->orientation) * glm::scale(identity, d->scale);
      // reset flag
      d->recalcTransformationMatrix = false;
    }
    return d->transformationMatrix;
  }
}
