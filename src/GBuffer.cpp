#include "GBuffer.h"

#include <GL/glew.h>

namespace SimpleGL {
  class GBufferPrivate {
  public:
    GBufferPrivate(unsigned int width, unsigned int height) : width(width), height(height), frameBuffer(0), colorBuffer(0), normalBuffer(0), depthBuffer(0) {
    }

    ~GBufferPrivate() {
    }

    GLuint width;
    GLuint height;
    GLuint frameBuffer;
    GLuint colorBuffer;
    GLuint normalBuffer;
    GLuint depthBuffer;
  };

  GBuffer::GBuffer(unsigned int width, unsigned int height) : d(new GBufferPrivate(width, height)) {
    // generate frame buffer
    glGenFramebuffers(1, &d->frameBuffer);
    // bind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
    // generate color buffer
    glGenTextures(1, &d->colorBuffer);
    glBindTexture(GL_TEXTURE_2D, d->colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, d->width, d->height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d->colorBuffer, 0);
    // generate normal buffer
    glGenTextures(1, &d->normalBuffer);
    glBindTexture(GL_TEXTURE_2D, d->normalBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, d->width, d->height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, d->normalBuffer, 0);
    // generate depth buffer
    glGenTextures(1, &d->depthBuffer);
    glBindTexture(GL_TEXTURE_2D, d->depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, d->width, d->height, 0, GL_RGB, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, d->depthBuffer, 0);
    // set draw buffer
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, drawBuffers);
    // unbind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  GBuffer::~GBuffer() {
    // delete position buffer
    glDeleteTextures(1, &d->depthBuffer);
    // delete normal buffer
    glDeleteTextures(1, &d->normalBuffer);
    // delete color buffer
    glDeleteTextures(1, &d->colorBuffer);
    // delete frame buffer
    glDeleteFramebuffers(1, &d->frameBuffer);
  }

  void GBuffer::select() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
  }

  void GBuffer::deselect() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  void GBuffer::blit() {
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
