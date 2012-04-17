#ifndef NODE_H
#define NODE_H

#include "TransformSpace.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>

namespace SimpleGL {
  class Light;
  class Mesh;
  class NodePrivate;

  class Node {
  public:
    Node();
    ~Node();

    Node *createChildNode(const glm::vec3 &position = glm::vec3(0, 0, 0), const glm::quat &orientation = glm::quat(1, 0, 0, 0), const glm::vec3 &scale = glm::vec3(1, 1, 1));

    void attachNode(Node *node);
    const std::vector<Node *> &nodes() const;

    void attachMesh(SimpleGL::Mesh *mesh);
    const std::vector<Mesh *> &meshes() const;

    void attachLight(Light *light);
    const std::vector<Light *> &lights() const;

    void setPosition(const glm::vec3 &position);
    void setPosition(float x, float y, float z);
    const glm::vec3 &position() const;

    void setOrientation(const glm::quat &orientation);
    void setOrientation(float w, float x, float y, float z);
    const glm::quat &orientation() const;

    void setScale(const glm::vec3 &scale);
    void setScale(float x, float y, float z);
    const glm::vec3 &scale() const;

    void rotate(float angle, const glm::vec3 &axis, TransformSpace transformSpace = TS_LOCAL);

    void pitch(float angle, TransformSpace transformSpace = TS_LOCAL);
    void yaw(float angle, TransformSpace transformSpace = TS_LOCAL);
    void roll(float angle, TransformSpace transformSpace = TS_LOCAL);

    const glm::mat4 &transformationMatrix() const;

  private:
    NodePrivate *d;
  };
}

#endif // NODE_H
