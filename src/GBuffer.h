#ifndef GBUFFER_H
#define	GBUFFER_H

#include "Types.h"

namespace SimpleGL {
  class GBufferPrivate;

  class GBuffer {
  public:
    GBuffer(uint width, uint height);
    ~GBuffer();

    void setWritable(bool writable);
    void bindTextures();
    void unbindTextures();

    const uint colorBuffer() const;
    const uint normalBuffer() const;
    const uint positionBuffer() const;

    void blit();

  private:
    GBufferPrivate *d;
  };
}

#endif // GBUFFER_H
