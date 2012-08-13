#include "FrameBuffer.h"

#include <vector>

#include <GL/glew.h>

namespace SimpleGL {
  class FrameBufferPrivate {
  public:
    FrameBufferPrivate(int width, int height) : width(width), height(height) {
    }

    ~FrameBufferPrivate() {
    }

    int width { 0 };
    int height { 0 };
    // frame buffer handle
    GLuint frameBuffer { 0 };
    // depth buffer handle
    GLuint depthBuffer { 0 };
    GLuint texture0 { 0 };
    GLuint texture1 { 0 };
    GLuint texture2 { 0 };
  };

  FrameBuffer::FrameBuffer(int width, int height) : d(new FrameBufferPrivate(width, height)) {
    // generate frame buffer
    glGenFramebuffers(1, &d->frameBuffer);
    // bind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
    // generate depth buffer
    glGenTextures(1, &d->depthBuffer);
    glBindTexture(GL_TEXTURE_2D, d->depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, d->width, d->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, d->depthBuffer, 0);
    // generate texture0
    glGenTextures(1, &d->texture0);
    glBindTexture(GL_TEXTURE_2D, d->texture0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d->texture0, 0);
    // generate texture1
    glGenTextures(1, &d->texture1);
    glBindTexture(GL_TEXTURE_2D, d->texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, d->texture1, 0);
    // generate texture2
    glGenTextures(1, &d->texture2);
    glBindTexture(GL_TEXTURE_2D, d->texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, d->texture2, 0);
    // set draw buffers
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, drawBuffers);
    // unbind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  FrameBuffer::~FrameBuffer() {
    // delete textures
    glDeleteTextures(1, &d->texture0);
    glDeleteTextures(1, &d->texture1);
    glDeleteTextures(1, &d->texture2);
    // delete depth buffer
    glDeleteTextures(1, &d->depthBuffer);
    // delete frame buffer
    glDeleteFramebuffers(1, &d->frameBuffer);
    // delete data
    delete d;
  }

  void FrameBuffer::bind() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
  }

  void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  void FrameBuffer::bindTextures() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, d->texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, d->texture1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, d->texture2);
  }

  void FrameBuffer::unbindTextures() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void FrameBuffer::blit() const {
    // bind frame buffer for reading
    glBindFramebuffer(GL_READ_FRAMEBUFFER, d->frameBuffer);
    // blit color buffer
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, d->width, d->height, 0, d->height / 2, d->width / 2, d->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    // blit normal buffer
    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(0, 0, d->width, d->height, d->width / 2, d->height / 2, d->width, d->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    // blit normal buffer
    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(0, 0, d->width, d->height, 0, 0, d->width / 2, d->height / 2, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    // unbind frame buffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  }
}
