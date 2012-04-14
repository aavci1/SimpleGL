#include "Cube.h"

#include "Attribute.h"

namespace SimpleGL {
  Cube::Cube(glm::vec3 size) : Mesh() {
    float vertices[] = {
      -1.0f * size.x, -1.0f * size.y,  1.0f * size.z, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,
       1.0f * size.x, -1.0f * size.y,  1.0f * size.z, 0.0f, 0.0f, +1.0f, 1.0f, 0.0f,
       1.0f * size.x,  1.0f * size.y,  1.0f * size.z, 0.0f, 0.0f, +1.0f, 1.0f, 1.0f,
      -1.0f * size.x,  1.0f * size.y,  1.0f * size.z, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f,

      -1.0f * size.x, -1.0f * size.y, -1.0f * size.z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
      -1.0f * size.x,  1.0f * size.y, -1.0f * size.z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
       1.0f * size.x,  1.0f * size.y, -1.0f * size.z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
       1.0f * size.x, -1.0f * size.y, -1.0f * size.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

      -1.0f * size.x,  1.0f * size.y, -1.0f * size.z, 0.0f, +1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f * size.x,  1.0f * size.y,  1.0f * size.z, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f,
       1.0f * size.x,  1.0f * size.y,  1.0f * size.z, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f,
       1.0f * size.x,  1.0f * size.y, -1.0f * size.z, 0.0f, +1.0f, 0.0f, 1.0f, 1.0f,

      -1.0f * size.x, -1.0f * size.y, -1.0f * size.z, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
       1.0f * size.x, -1.0f * size.y, -1.0f * size.z, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
       1.0f * size.x, -1.0f * size.y,  1.0f * size.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      -1.0f * size.x, -1.0f * size.y,  1.0f * size.z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,

       1.0f * size.x, -1.0f * size.y, -1.0f * size.z, +1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
       1.0f * size.x,  1.0f * size.y, -1.0f * size.z, +1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
       1.0f * size.x,  1.0f * size.y,  1.0f * size.z, +1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
       1.0f * size.x, -1.0f * size.y,  1.0f * size.z, +1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

      -1.0f * size.x, -1.0f * size.y, -1.0f * size.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      -1.0f * size.x, -1.0f * size.y,  1.0f * size.z, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      -1.0f * size.x,  1.0f * size.y,  1.0f * size.z, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      -1.0f * size.x,  1.0f * size.y, -1.0f * size.z, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    uint indices[] = {
      0, 1, 2,
      0, 2, 3,

      4, 5, 6,
      4, 6, 7,

      8, 9, 10,
      8, 10, 11,

      12, 13, 14,
      12, 14, 15,

      16, 17, 18,
      16, 18, 19,

      20, 21, 22,
      20, 22, 23
    };

    setVertexData(SGL_POSITION | SGL_NORMAL | SGL_TEXCOORD0, vertices, 24, 32);
    setIndexData(indices, 36);
  }
}
