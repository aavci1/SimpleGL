#include "Node.h"

#include "Light.h"
#include "Mesh.h"

#include <glm/ext.hpp>

namespace SimpleGL {
  class NodePrivate {
  public:
    NodePrivate() {
    }

    ~NodePrivate() {
    }

    std::vector<Node *> nodes;
    std::vector<Mesh *> meshes;
    std::vector<Light *> lights;
    glm::mat4 transformationMatrix;
  };

  Node::Node() : d(new NodePrivate()) {
  }

  Node::~Node() {
    delete d;
  }

  void Node::attachNode(Node *node) {
    d->nodes.push_back(node);
  }

  const std::vector<Node *> &Node::nodes() {
    return d->nodes;
  }

  void Node::attachMesh(Mesh *mesh) {
    d->meshes.push_back(mesh);
  }

  const std::vector<Mesh *> &Node::meshes() {
    return d->meshes;
  }

  void Node::attachLight(Light *light) {
    d->lights.push_back(light);
  }

  const std::vector<Light *> &Node::lights() {
    return d->lights;
  }

  void Node::translate(glm::vec3 translation) {
    d->transformationMatrix = glm::translate(d->transformationMatrix, translation);
  }

  void Node::rotate(glm::vec3 axis, float angle) {
    d->transformationMatrix = glm::rotate(d->transformationMatrix, angle, axis);
  }

  void Node::scale(glm::vec3 scale) {
    d->transformationMatrix = glm::scale(d->transformationMatrix, scale);
  }

  const glm::mat4 &Node::transformationMatrix() {
    return d->transformationMatrix;
  }
}
