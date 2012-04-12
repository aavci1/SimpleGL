#ifndef GBUFFER_H
#define	GBUFFER_H

#include <sys/types.h>

namespace SimpleGL {
  class GBufferPrivate;


  class GBuffer {
  public:
    GBuffer(uint width, uint height);
    ~GBuffer();

    void select();
    void deselect();

    void blit();

  private:
    GBufferPrivate *d;
  };
}

#endif // GBUFFER_H
