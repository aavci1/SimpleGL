#include "GBuffer.h"

#include <GL/glew.h>

namespace SimpleGL {
  class GBufferPrivate {
  public:
    GBufferPrivate(unsigned int width, unsigned int height) : width(width), height(height), frameBuffer(0), colorBuffer(0), normalBuffer(0), positionBuffer(0), depthBuffer(0) {
    }

    ~GBufferPrivate() {
    }

    GLuint width;
    GLuint height;
    GLuint frameBuffer;
    GLuint colorBuffer;
    GLuint normalBuffer;
    GLuint positionBuffer;
    GLuint depthBuffer;
  };

  GBuffer::GBuffer(unsigned int width, unsigned int height) : d(new GBufferPrivate(width, height)) {
    // generate frame buffer
    glGenFramebuffers(1, &d->frameBuffer);
    // bind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
    // generate depth buffer
    glGenTextures(1, &d->depthBuffer);
    glBindTexture(GL_TEXTURE_2D, d->depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, d->width, d->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, d->depthBuffer, 0);
    // generate color buffer
    glGenTextures(1, &d->colorBuffer);
    glBindTexture(GL_TEXTURE_2D, d->colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, d->colorBuffer, 0);
    // generate normal buffer
    glGenTextures(1, &d->normalBuffer);
    glBindTexture(GL_TEXTURE_2D, d->normalBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, d->normalBuffer, 0);
    // generate position buffer
    glGenTextures(1, &d->positionBuffer);
    glBindTexture(GL_TEXTURE_2D, d->positionBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, d->positionBuffer, 0);
    // set draw buffer
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, drawBuffers);
    // unbind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  GBuffer::~GBuffer() {
    // delete depth buffer
    glDeleteTextures(1, &d->depthBuffer);
    // delete position buffer
    glDeleteTextures(1, &d->positionBuffer);
    // delete normal buffer
    glDeleteTextures(1, &d->normalBuffer);
    // delete color buffer
    glDeleteTextures(1, &d->colorBuffer);
    // delete frame buffer
    glDeleteFramebuffers(1, &d->frameBuffer);
    //
    delete d;
  }

  const uint GBuffer::colorBuffer() const {
    return 0;
  }

  const uint GBuffer::normalBuffer() const {
    return 1;
  }

  const uint GBuffer::positionBuffer() const {
    return 2;
  }

  void GBuffer::setWritable(bool writable) {
    if (writable)
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
    else
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  void GBuffer::bindTextures() {
    // bind color buffer
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, d->colorBuffer);
    // bind normal buffer
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, d->normalBuffer);
    // bind position buffer
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, d->positionBuffer);
  }

  void GBuffer::unbindTextures() {
    // unbind color buffer
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    // unbind normal buffer
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    // unbind position buffer
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
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
