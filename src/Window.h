#ifndef WINDOW_H
#define WINDOW_H

#include "Types.h"

namespace SimpleGL {
  class WindowPrivate;
  class Camera;
  class Viewport;

  class Window {
  public:
    Window(int width, int height);
    ~Window();

    uint32_t width();
    uint32_t height();
    void setSize(const int width, const int height);

    uint32_t numViewports() const;
    Viewport *viewportAt(int index) const;
    Viewport *createViewport(Camera *camera);

    void update();

  private:
    WindowPrivate *d;
  };
}

#endif // WINDOW_H
