#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Types.h"

namespace SimpleGL {
  class FrameBufferPrivate;

  class FrameBuffer {
  public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void createTexture(TextureFormat textureFormat);

    void bind() const;
    void unbind() const;

    void bindTextures() const;
    void unbindTextures() const;

    void blit() const;

  private:
    FrameBufferPrivate *d;
  };
}

#endif // FRAMEBUFFER_H
