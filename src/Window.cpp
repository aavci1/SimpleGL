#include "Window.h"

#include "Camera.h"
#include "Root.h"
#include "Viewport.h"

#include <GL/glew.h>

#include <algorithm>

namespace SimpleGL {
  class WindowPrivate {
  public:
    WindowPrivate() {
      // generate frame buffer
      glGenFramebuffers(1, &frameBuffer);
    }

    ~WindowPrivate() {
      // delete frame buffer
      glDeleteFramebuffers(1, &frameBuffer);
      // delete viewports
      for (uint i = 0; i < viewports.size(); ++i)
        delete viewports[i];
    }

    std::vector<Viewport *> viewports;
    uint32_t width { 0 };
    uint32_t height { 0 };
    // frame buffer handle
    GLuint frameBuffer { 0 };
    // depth buffer handle
    GLuint depthBuffer { 0 };
    GLuint texture0 { 0 };
    GLuint texture1 { 0 };
    GLuint texture2 { 0 };
  };

  Window::Window(int width, int height) : d(new WindowPrivate()) {
    // generate textures
    setSize(width, height);
  }

  Window::~Window() {
    delete d;
  }

  uint32_t Window::width() {
    return d->width;
  }

  uint32_t Window::height() {
    return d->height;
  }

  void Window::setSize(const int width, const int height) {
    d->width = width;
    d->height = height;
    // delete depth buffer
    glDeleteTextures(1, &d->depthBuffer);
    // generate depth buffer
    glGenTextures(1, &d->depthBuffer);
    glBindTexture(GL_TEXTURE_2D, d->depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, d->width, d->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    // delete texture0
    glDeleteTextures(1, &d->texture0);
    // generate texture0
    glGenTextures(1, &d->texture0);
    glBindTexture(GL_TEXTURE_2D, d->texture0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    // delete texture1
    glDeleteTextures(1, &d->texture1);
    // generate texture1
    glGenTextures(1, &d->texture1);
    glBindTexture(GL_TEXTURE_2D, d->texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    // delete texture2
    glDeleteTextures(1, &d->texture2);
    // generate texture2
    glGenTextures(1, &d->texture2);
    glBindTexture(GL_TEXTURE_2D, d->texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    // bind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
    // attach depth buffer
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, d->depthBuffer, 0);
    // attach textures
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d->texture0, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, d->texture1, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, d->texture2, 0);
    // set draw buffers
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, drawBuffers);
    // unbind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  uint32_t Window::numViewports() const {
    return d->viewports.size();
  }

  Viewport *Window::viewportAt(int index) const {
    return d->viewports.at(index);
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
    // bind frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
    // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // unbind framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // render through each viewport
    for (Viewport *viewport: d->viewports) {
      // calculate viewport dimension in pixels;
      float left = viewport->left() * d->width;
      float top = viewport->top() * d->height;
      float width = viewport->width() * d->width;
      float height = viewport->height() * d->height;
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
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
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
      // unbind frame buffer
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
#if 0
      // bind frame buffer for reading
      glBindFramebuffer(GL_READ_FRAMEBUFFER, d->frameBuffer);
      // blit color buffer
      glReadBuffer(GL_COLOR_ATTACHMENT0);
      glBlitFramebuffer(0, 0, d->width, d->height, 0, d->height / 2, d->width / 2, d->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
      // blit normal buffer
      glReadBuffer(GL_COLOR_ATTACHMENT1);
      glBlitFramebuffer(0, 0, d->width, d->height, d->width / 2, d->height / 2, d->width, d->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
      // blit position buffer
      glReadBuffer(GL_COLOR_ATTACHMENT2);
      glBlitFramebuffer(0, 0, d->width, d->height, 0, 0, d->width / 2, d->height / 2, GL_COLOR_BUFFER_BIT, GL_LINEAR);
      // unbind frame buffer
      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
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
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, d->texture0);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, d->texture1);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, d->texture2);
      // render lights
      Root::instance()->renderLights(this, viewport);
      // unbind textures
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, 0);
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, 0);
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, 0);
#endif
    }
  }
}
