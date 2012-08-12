#include "Renderable.h"

namespace SimpleGL {
  class RenderablePrivate {
  public:
    RenderablePrivate() {
    }

    ~RenderablePrivate() {
    }

    String material { "" };
  };

  Renderable::Renderable() : d(new RenderablePrivate()) {
  }

  Renderable::~Renderable() {
    delete d;
  }

  const String &Renderable::material() const {
    return d->material;
  }

  void Renderable::setMaterial(const String &name) {
    d->material = name;
  }
}
