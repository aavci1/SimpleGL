#include "Viewport.h"

namespace SimpleGL {
  class ViewportPrivate {
  public:
    ViewportPrivate(Camera *camera) : camera(camera) {
    }

    ~ViewportPrivate() {
    }

    Camera *camera { nullptr };

    float left { 0.0f };
    float top { 0.0f };
    float width { 1.0f };
    float height { 1.0f };
    int zIndex { 0 };
  };

  Viewport::Viewport(Camera *camera) : d(new ViewportPrivate(camera)) {
  }

  Viewport::~Viewport() {
    delete d;
  }

  Camera *Viewport::camera() const {
    return d->camera;
  }

  void Viewport::setCamera(Camera *camera) {
    d->camera = camera;
  }

  const float Viewport::left() const {
    return d->left;
  }

  const float Viewport::top() const {
    return d->top;
  }

  const float Viewport::width() const {
    return d->width;
  }

  const float Viewport::height() const {
    return d->height;
  }

  void Viewport::setBounds(float left, float top, float width, float height) {
    d->left = left;
    d->top = top;
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
