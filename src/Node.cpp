#include "Node.h"

#include "Light.h"
#include "Mesh.h"

namespace SimpleGL {
  class NodePrivate {
  public:
    NodePrivate() : position(0, 0, 0), orientation(1, 0, 0, 0), scale(1, 1, 1), recalcTransformationMatrix(false) {
    }

    ~NodePrivate() {
    }

    std::vector<Node *> nodes;
    std::vector<Mesh *> meshes;
    std::vector<Light *> lights;
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 scale;
    bool recalcTransformationMatrix;
    glm::mat4 transformationMatrix;
  };

  Node::Node() : d(new NodePrivate()) {
  }

  Node::~Node() {
    delete d;
  }

  Node *Node::createChildNode() {
    Node *childNode = new Node();
    d->nodes.push_back(childNode);
    return childNode;
  }

  void Node::attachNode(Node *node) {
    d->nodes.push_back(node);
  }

  const std::vector<Node *> &Node::nodes() const {
    return d->nodes;
  }

  void Node::attachMesh(Mesh *mesh) {
    d->meshes.push_back(mesh);
  }

  const std::vector<Mesh *> &Node::meshes() const {
    return d->meshes;
  }

  void Node::attachLight(Light *light) {
    d->lights.push_back(light);
  }

  const std::vector<Light *> &Node::lights() const {
    return d->lights;
  }

  void Node::setPosition(const glm::vec3 &position) {
    d->position = position;
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void Node::setPosition(float x, float y, float z) {
    d->position = glm::vec3(x, y, z);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::vec3 &Node::position() const {
    return d->position;
  }

  void Node::setOrientation(const glm::quat &orientation) {
    d->orientation = glm::normalize(orientation);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void Node::setOrientation(float w, float x, float y, float z) {
    d->orientation = glm::normalize(glm::quat(w, x, y, z));
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::quat &Node::orientation() const {
    return d->orientation;
  }

  void Node::setScale(const glm::vec3 &scale) {
    d->scale = scale;
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void Node::setScale(float x, float y, float z) {
    d->scale = glm::vec3(x, y, z);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::vec3 &Node::scale() const {
    return d->scale;
  }

  void Node::rotate(float angle, const glm::vec3 &axis, TransformSpace transformSpace) {
    if (transformSpace == TS_WORLD)
      setOrientation(glm::angleAxis(angle, axis) * d->orientation);
    else if (transformSpace == TS_LOCAL)
      setOrientation(d->orientation * glm::angleAxis(angle, axis));
  }

  void Node::pitch(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
  }

  void Node::yaw(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
  }

  void Node::roll(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
  }

  const glm::mat4 &Node::transformationMatrix() const {
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
