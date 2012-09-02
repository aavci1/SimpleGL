#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Types.h"

namespace SimpleGL {
  class RenderablePrivate;
  class Camera;

  class Renderable {
  public:
    Renderable();
    virtual ~Renderable();

    const string &material() const;
    void setMaterial(const string &name);

    virtual void render(Camera *camera) = 0;

  private:
    RenderablePrivate *d;
  };
}
#endif // RENDERABLE_H
