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
    // color buffers
    std::vector<GLuint> colorBuffers;
  };

  FrameBuffer::FrameBuffer(int width, int height) : d(new FrameBufferPrivate(width, height)) {
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
    // unbind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  FrameBuffer::~FrameBuffer() {
    // delete color buffers
    for (uint i = 0; i < d->colorBuffers.size(); ++i)
      glDeleteTextures(1, &d->colorBuffers.at(i));
    // delete depth buffer
    glDeleteTextures(1, &d->depthBuffer);
    // delete frame buffer
    glDeleteFramebuffers(1, &d->frameBuffer);
    // delete data
    delete d;
  }

  void FrameBuffer::bind() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
    // set draw buffers
    GLenum *drawBuffers = new GLenum[d->colorBuffers.size()];
    for (uint i = 0; i < d->colorBuffers.size(); ++i)
      drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
    glDrawBuffers(d->colorBuffers.size(), drawBuffers);
    delete[] drawBuffers;
  }

  void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  void FrameBuffer::createTexture(TextureFormat textureFormat) {
    GLuint colorBuffer;
    // bind the frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, d->frameBuffer);
    // generate color buffer
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, (textureFormat == TF_RGBA8) ? GL_RGBA8 : GL_RGBA16F, d->width, d->height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + d->colorBuffers.size(), GL_TEXTURE_2D, colorBuffer, 0);
    // add to list
    d->colorBuffers.push_back(colorBuffer);
    // unbind frame buffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  }

  void FrameBuffer::bindTextures() const {
    for (uint i = 0; i < d->colorBuffers.size(); ++i) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, d->colorBuffers.at(i));
    }
  }

  void FrameBuffer::unbindTextures() const {
    for (uint i = 0; i < d->colorBuffers.size(); ++i) {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, 0);
    }
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
