#ifndef GBUFFER_H
#define	GBUFFER_H

#include <sys/types.h>

namespace SimpleGL {
  class GBufferPrivate;

  class GBuffer {
  public:
    GBuffer(uint width, uint height);
    ~GBuffer();

    void setWritable(bool writable);
    void bindTextures();
    void unbindTextures();

    const uint colorSampler() const;
    const uint normalSampler() const;
    const uint positionSampler() const;

    void blit();

  private:
    GBufferPrivate *d;
  };
}

#endif // GBUFFER_H
