#include "Window.h"

#include "FrameBuffer.h"
#include "Viewport.h"

#include <GL/glew.h>

#include <algorithm>

namespace SimpleGL {
  class WindowPrivate {
  public:
    WindowPrivate() : size(0, 0) {
    }

    ~WindowPrivate() {
      // delete frame buffer
      delete frameBuffer;
      // delete viewports
      for (uint i = 0; i < viewports.size(); ++i)
        delete viewports[i];
    }

    Vector2i size;
    FrameBuffer *frameBuffer = nullptr;
    std::vector<Viewport *> viewports;
  };

  Window::Window(int width, int height) : d(new WindowPrivate()) {
    setSize(width, height);
  }

  Window::~Window() {
    delete d;
  }

  FrameBuffer *Window::frameBuffer() const {
    return d->frameBuffer;
  }

  const Vector2i &Window::size() const {
    return d->size;
  }

  void Window::setSize(const Vector2i &size) {
    d->size = size;
    // delete old frame buffer
    delete d->frameBuffer;
    // create frame buffer
    d->frameBuffer = new FrameBuffer(size.x, size.y);
    // create color buffer
    d->frameBuffer->createTexture(TF_RGBA8);
    // create normal buffer
    d->frameBuffer->createTexture(TF_RGBA16F);
    // create position buffer
    d->frameBuffer->createTexture(TF_RGBA16F);
  }

  void Window::setSize(const int width, const int height) {
    setSize(Vector2i(width, height));
  }

  const std::vector<Viewport *> &Window::viewports() const {
    return d->viewports;
  }

  Viewport *Window::createViewport(Camera *camera) {
    Viewport *viewport = new Viewport(camera);
    // add to list
    d->viewports.push_back(viewport);
    // sort viewports
    std::sort(d->viewports.begin(), d->viewports.end(), [](const Viewport * v1, const Viewport * v2) {
      return v1->zIndex() < v2->zIndex();
    });
    // return viewport
    return viewport;
  }
}
