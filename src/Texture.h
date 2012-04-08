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

    std::string path();

    bool load();

    bool select(ushort index);
    bool deselect();

  private:
    TexturePrivate *d;
  };
}

#endif // TEXTURE_H
