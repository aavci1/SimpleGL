#ifndef TEXTURE_H
#define TEXTURE_H

#include "Types.h"

namespace SimpleGL {
  class TexturePrivate;

  class Texture {
  public:
    Texture(const String &name, const String &path);
    ~Texture();

    const String &name() const;
    const String &path() const;

    const bool bind(uint index) const;
    const bool unbind() const;

  private:
    TexturePrivate *d;
  };
}

#endif // TEXTURE_H
