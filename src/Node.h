#ifndef NODE_H
#define NODE_H

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

    void attachNode(Node *node);
    const std::vector<Node *> &nodes() const;

    void attachMesh(Mesh *mesh);
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

    void rotate(float angle, const glm::vec3 &axis);

    void pitch(float angle);
    void yaw(float angle);
    void roll(float angle);

    const glm::mat4 &transformationMatrix() const;

  private:
    NodePrivate *d;
  };
}

#endif // NODE_H
