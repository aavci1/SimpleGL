#include "Renderable.h"

namespace SimpleGL {
  class RenderablePrivate {
  public:
    RenderablePrivate() : material("") {
    }

    ~RenderablePrivate() {
    }

    String material;
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
