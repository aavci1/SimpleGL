#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Types.h"

namespace SimpleGL {
  class FrameBufferPrivate;

  class FrameBuffer {
  public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void createColorBuffer(TextureFormat textureFormat);
    const uint numColorBuffers() const;

    void bind() const;
    void unbind() const;

    void bindColorBuffers() const;
    void unbindColorBuffers() const;

    void blit() const;

  private:
    FrameBufferPrivate *d;
  };
}

#endif // FRAMEBUFFER_H
