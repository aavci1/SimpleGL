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

    const float x() const;
    const float y() const;
    const float width() const;
    const float height() const;
    void setBounds(float x, float y, float width, float height);

    const int zIndex() const;
    void setZIndex(int zIndex);

  private:
    ViewportPrivate *d;
  };
}

#endif // VIEWPORT_H
