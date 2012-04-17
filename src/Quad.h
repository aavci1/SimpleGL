#ifndef QUAD_H
#define QUAD_H

#include "Mesh.h"

#include <glm/glm.hpp>

namespace SimpleGL {
  class Quad : public Mesh {
  public:
    Quad(glm::vec2 size = glm::vec2(1.0f, 1.0f));

    void render();
  };
}

#endif // QUAD_H
