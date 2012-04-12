#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>

namespace SimpleGL {
  class Camera;
  class Node;

  class Renderer {
  public:
    Renderer();
    ~Renderer();

    virtual void renderOneFrame(Camera *camera, Node *root) = 0;
  };
}
#endif // RENDERER_H
