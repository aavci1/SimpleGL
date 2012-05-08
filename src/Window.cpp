#include "Window.h"

#include "FrameBuffer.h"
#include "Viewport.h"

#include <GL/glew.h>
#include <GL/glfw.h>

#include <algorithm>

namespace SimpleGL {
  class WindowPrivate {
  public:
    WindowPrivate() : title(""), size(0, 0), frameBuffer(0) {
    }

    ~WindowPrivate() {
      // delete frame buffer
      delete frameBuffer;
      // delete viewports
      for (uint i = 0; i < viewports.size(); ++i)
        delete viewports[i];
    }

    String title;
    Vector2i size;
    FrameBuffer *frameBuffer;
    std::vector<Viewport *> viewports;
  };

  Window::Window(int width, int height, bool fullscreen, bool stereo) : d(new WindowPrivate) {
    // set up stereo
    glfwOpenWindowHint(GLFW_STEREO, stereo ? GL_TRUE : GL_FALSE);
    // create a window
    if (glfwOpenWindow(width, height, 8, 8, 8, 0, 24, 0, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW) != GL_TRUE) {
      std::cerr << "error: can not create render window." << std::endl;
      // return fail
      return;
    }
    // initialize GLEW
    if (glewInit() != GLEW_OK) {
      std::cerr << "error: can not initialize GLEW." << std::endl;
      // destroy render window
      glfwCloseWindow();
      // return fail
      return;
    }
    // print some info about OpenGL version etc.
    std::cout << "OpenGL vendor       : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL renderer     : " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version      : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version        : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Supported extensions: " << glGetString(GL_EXTENSIONS) << std::endl;
    // call resize event
    resizeEvent(width, height);
  }

  Window::~Window() {
    delete d;
    // close the window
    glfwCloseWindow();
  }

  void Window::resizeEvent(int width, int height) {
    // update size
    d->size.x = width;
    d->size.y = height;
    // delete old frame buffer
    delete d->frameBuffer;
    // create frame buffer
    d->frameBuffer = new FrameBuffer(width, height);
    // create color buffer
    d->frameBuffer->createTexture(TF_RGBA8);
    // create normal buffer
    d->frameBuffer->createTexture(TF_RGBA16F);
    // create position buffer
    d->frameBuffer->createTexture(TF_RGBA16F);
  }

  const bool Window::isOpen() const {
    return glfwGetWindowParam(GLFW_OPENED) == GL_TRUE;
  }

  const Vector2i &Window::size() const {
    return d->size;
  }

  void Window::setSize(const Vector2i &size) {
    glfwSetWindowSize(size.x, size.y);
  }

  void Window::setSize(const int width, const int height) {
    setSize(Vector2i(width, height));
  }

  const String &Window::title() const {
    return d->title;
  }

  void Window::setTitle(const String &title) const {
    glfwSetWindowTitle(title.c_str());
  }

  bool viewportSorter(Viewport *v1, Viewport *v2) {
    return v1->zIndex() < v2->zIndex();
  }

  Viewport *Window::createViewport(Camera *camera) {
    Viewport *viewport = new Viewport(camera);
    // add to list
    d->viewports.push_back(viewport);
    // sort viewports
    std::sort(d->viewports.begin(), d->viewports.end(), viewportSorter);
    // return viewport
    return viewport;
  }

  const std::vector<Viewport *> &Window::viewports() const {
    return d->viewports;
  }

  FrameBuffer *Window::frameBuffer() const {
    return d->frameBuffer;
  }
}
