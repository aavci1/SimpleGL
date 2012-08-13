#include "Window.h"

#include "Camera.h"
#include "FrameBuffer.h"
#include "Root.h"
#include "Viewport.h"

#include <GL/glew.h>

#include <algorithm>

namespace SimpleGL {
  class WindowPrivate {
  public:
    WindowPrivate() {
    }

    ~WindowPrivate() {
      // delete frame buffer
      delete frameBuffer;
      // delete viewports
      for (uint i = 0; i < viewports.size(); ++i)
        delete viewports[i];
    }

    Vector2i size { 0, 0 };
    FrameBuffer *frameBuffer { nullptr };
    std::vector<Viewport *> viewports;
  };

  Window::Window(int width, int height) : d(new WindowPrivate()) {
    setSize(width, height);
  }

  Window::~Window() {
    delete d;
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

  void Window::update() {
    // set general state
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    // update scene transformations
    Root::instance()->calculateWorldTransforms();
    // clear color and depth buffers
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // bind gbuffer for writing
    d->frameBuffer->bind();
    // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // unbind framebuffer
    d->frameBuffer->unbind();
    // render through each viewport
    for (Viewport *viewport: d->viewports) {
      // calculate viewport dimension in pixels;
      float left = viewport->left() * d->size.x;
      float top = viewport->top() * d->size.y;
      float width = viewport->width() * d->size.x;
      float height = viewport->height() * d->size.y;
      // set up viewport
      glViewport(left, top, width, height);
      // get viewport camera
      Camera *camera = viewport->camera();
      if (!camera)
        continue;
      // update camera's aspect ratio
      camera->setAspectRatio(width / height);
      /////////////////////////////////////////////////////////////////////////////
      ////  GEOMETRY PASS
      /////////////////////////////////////////////////////////////////////////////
      d->frameBuffer->bind();
      // enable face culling
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      // enable depth test
      glEnable(GL_DEPTH_TEST);
      glDepthMask(GL_TRUE);
      glDepthFunc(GL_LESS);
      // disable blending
      glDisable(GL_BLEND);
      // render scene
      Root::instance()->renderScene(this, viewport);
      // unbind gbuffer
      d->frameBuffer->unbind();
#if 0
      // blit the frame buffer
      d->frameBuffer->blit();
#else
      /////////////////////////////////////////////////////////////////////////////
      ////  LIGHTING PASS
      /////////////////////////////////////////////////////////////////////////////
      // enable face culling
      glEnable(GL_CULL_FACE);
      glCullFace(GL_BACK);
      // disable depth test
      glDisable(GL_DEPTH_TEST);
      glDepthMask(GL_FALSE);
      // enable blending
      glEnable(GL_BLEND);
      glBlendEquation(GL_FUNC_ADD);
      glBlendFunc(GL_ONE, GL_ONE);
      // bind textures
      d->frameBuffer->bindTextures();
      // render lights
      Root::instance()->renderLights(this, viewport);
      // unbind textures
      d->frameBuffer->unbindTextures();
#endif
    }
  }
}
