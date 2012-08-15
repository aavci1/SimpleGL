#include "Renderable.h"

namespace SimpleGL {
  class RenderablePrivate {
  public:
    RenderablePrivate() {
    }

    ~RenderablePrivate() {
    }

    string material { "" };
  };

  Renderable::Renderable() : d(new RenderablePrivate()) {
  }

  Renderable::~Renderable() {
    delete d;
  }

  const string &Renderable::material() const {
    return d->material;
  }

  void Renderable::setMaterial(const string &name) {
    d->material = name;
  }
}
