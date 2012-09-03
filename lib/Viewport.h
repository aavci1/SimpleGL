#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Types.h"

namespace SimpleGL {
  class ViewportPrivate;

  class Viewport {
  public:
    Viewport(CameraPtr camera);
    ~Viewport();

    CameraPtr camera() const;
    void setCamera(CameraPtr camera);

    const float left() const;
    const float top() const;
    const float width() const;
    const float height() const;
    void setBounds(float left, float top, float width, float height);

    const int zIndex() const;
    void setZIndex(int zIndex);

  private:
    ViewportPrivate *d;
  };
}

#endif // VIEWPORT_H
