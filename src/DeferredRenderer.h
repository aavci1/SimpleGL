#ifndef DEFERREDRENDERER_H
#define DEFERREDRENDERER_H

#include "Renderer.h"

#include <sys/types.h>

namespace SimpleGL {
  class DeferredRendererPrivate;

  class DeferredRenderer : public Renderer {
  public:
    DeferredRenderer(uint width, uint height);
    ~DeferredRenderer();

    void renderOneFrame(Camera *camera, Node *root);

  private:
    DeferredRendererPrivate *d;
  };
}

#endif // DEFERREDRENDERER_H
