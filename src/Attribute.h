#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

namespace SimpleGL {
  enum Attributes {
    SGL_POSITION = 0x00001,
    SGL_NORMAL = 0x0002,
    SGL_COLOR = 0x0004,
    SGL_TEXCOORD0 = 0x0008,
    SGL_TEXCOORD1 = 0x0010,
    SGL_TEXCOORD2 = 0x0020,
    SGL_CUSTOM = 0x8000
  };
}

#endif // ATTRIBUTE_H
