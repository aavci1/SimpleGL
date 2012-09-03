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

    const vector<shared_ptr<Viewport>> &viewports() const;
    shared_ptr<Viewport> createViewport(shared_ptr<Camera> camera);

    void update();

  private:
    WindowPrivate *d;
  };
}

#endif // WINDOW_H
