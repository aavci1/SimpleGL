#ifndef VIEWPORT_H
#define VIEWPORT_H

namespace SimpleGL {
  class ViewportPrivate;
  class Camera;

  class Viewport {
  public:
    Viewport(Camera *camera);
    ~Viewport();

    Camera *camera() const;
    void setCamera(Camera *camera);

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
