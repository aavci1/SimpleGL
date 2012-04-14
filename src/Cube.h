#ifndef CUBE_H
#define CUBE_H

#include "Mesh.h"

#include <glm/glm.hpp>

namespace SimpleGL {
  class Cube : public Mesh {
  public:
    Cube(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
    ~Cube();
  };
}

#endif // CUBE_H
