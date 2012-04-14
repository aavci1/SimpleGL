#ifndef PLANE_H
#define PLANE_H

#include "Mesh.h"

#include <glm/glm.hpp>

namespace SimpleGL {
  class Plane : public Mesh {
  public:
    Plane(glm::vec2 size, glm::vec2 uvTile = glm::vec2(1.0f, 1.0f));
    ~Plane();
  };
}

#endif // PLANE_H
