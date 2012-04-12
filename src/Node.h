#ifndef NODE_H
#define NODE_H

#include <glm/glm.hpp>

#include <vector>

namespace SimpleGL {
  class Mesh;
  class NodePrivate;

  class Node {
  public:
    Node();
    ~Node();

    void attachNode(Node *node);
    const std::vector<Node *> &nodes();

    void attachMesh(Mesh *mesh);
    const std::vector<Mesh *> &meshes();

    void translate(glm::vec3 translation);
    void rotate(glm::vec3 axis, float angle);
    void scale(glm::vec3 scale);

    const glm::mat4 &transformationMatrix();

  private:
    NodePrivate *d;
  };
}

#endif // NODE_H
