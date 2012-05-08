#ifndef WINDOW_H
#define WINDOW_H

#include "Types.h"

#include <vector>

namespace SimpleGL {
  class WindowPrivate;
  class Camera;
  class FrameBuffer;
  class Viewport;

  class Window {
  public:
    Window(int width, int height, bool fullscreen, bool stereo);
    ~Window();

    void resizeEvent(int width, int height);

    const bool isOpen() const;

    const Vector2i &size() const;
    void setSize(const Vector2i &size);
    void setSize(const int width, const int height);

    const String &title() const;
    void setTitle(const String &title) const;

    Viewport *createViewport(Camera *camera);
    const std::vector<Viewport *> &viewports() const;

    FrameBuffer *frameBuffer() const;
  private:
    WindowPrivate *d;
  };
}

#endif // WINDOW_H
