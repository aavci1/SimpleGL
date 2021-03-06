#ifndef WINDOW_H
#define WINDOW_H

#include "Types.h"

namespace SimpleGL {
  class WindowPrivate;

  class Window {
  public:
    Window(int width, int height);
    ~Window();

    uint32_t width();
    uint32_t height();
    void setSize(const int width, const int height);

    const vector<ViewportPtr> &viewports() const;
    ViewportPtr createViewport(CameraPtr camera);

    void update();

  private:
    WindowPrivate *d;
  };
}

#endif // WINDOW_H
