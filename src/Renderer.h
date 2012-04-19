#ifndef RENDERER_H
#define RENDERER_H

#include "Types.h"

#include <string>

namespace SimpleGL {
  class RendererPrivate;

  class Camera;
  class Node;

  class Renderer {
  public:
    Renderer(uint width, uint height);
    ~Renderer();

    void setSize(uint width, uint height);

    void renderOneFrame(Camera *camera, Node *root);

    bool saveScreenshot(const std::string &path);

  private:
    RendererPrivate *d;
  };
}

#endif // RENDERER_H
