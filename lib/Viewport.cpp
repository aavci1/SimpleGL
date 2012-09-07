#include "Viewport.h"

namespace SimpleGL {
  class ViewportPrivate {
  public:
    ViewportPrivate() {
    }

    ~ViewportPrivate() {
    }

    CameraPtr camera { nullptr };

    float x { 0.0f };
    float y { 0.0f };
    float width { 1.0f };
    float height { 1.0f };
    int zIndex { 0 };
  };

  Viewport::Viewport(CameraPtr camera) : d(new ViewportPrivate()) {
    d->camera = camera;
  }

  Viewport::~Viewport() {
    delete d;
  }

  CameraPtr Viewport::camera() const {
    return d->camera;
  }

  void Viewport::setCamera(CameraPtr camera) {
    d->camera = camera;
  }

  const float Viewport::x() const {
    return d->x;
  }

  const float Viewport::y() const {
    return d->y;
  }

  const float Viewport::width() const {
    return d->width;
  }

  const float Viewport::height() const {
    return d->height;
  }

  void Viewport::setBounds(float x, float y, float width, float height) {
    d->x = x;
    d->y = y;
    d->width = width;
    d->height = height;
  }

  const int Viewport::zIndex() const {
    return d->zIndex;
  }

  void Viewport::setZIndex(int zIndex) {
    d->zIndex = zIndex;
  }
}
