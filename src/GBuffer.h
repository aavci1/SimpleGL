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
    void setReadable(bool readable);

    void blit();

  private:
    GBufferPrivate *d;
  };
}

#endif // GBUFFER_H
