#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <sys/types.h>

namespace SimpleGL {
  class TexturePrivate;

  class Texture {
  public:
    Texture(std::string path);
    ~Texture();

    uint id();
    uint sampler();
    uint width();
    uint height();
    uint bitsPerPixel();
    std::string path();

    bool load();

  private:
    TexturePrivate *d;
  };
}

#endif // TEXTURE_H
