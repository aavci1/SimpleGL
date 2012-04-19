#ifndef TYPES_H
#define TYPES_H

#include <sys/types.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace SimpleGL {
  enum AttributeType {
    SGL_POSITION = 0x00001,
    SGL_NORMAL = 0x0002,
    SGL_COLOR = 0x0004,
    SGL_TEXCOORD0 = 0x0008,
    SGL_TEXCOORD1 = 0x0010,
    SGL_TEXCOORD2 = 0x0020,
    SGL_CUSTOM = 0x8000
  };

  enum LightType {
    LT_UNKNOWN,
    LT_DIRECTIONAL,
    LT_POINT,
    LT_SPOT
  };

  enum TransformSpace {
    TS_LOCAL,
    TS_WORLD
  };
}

#endif // TYPES_H
