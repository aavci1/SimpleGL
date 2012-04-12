#ifndef PLANEMESH_H
#define PLANEMESH_H

#include "Mesh.h"

#include <glm/glm.hpp>

namespace SimpleGL {
  class PlaneMesh : public Mesh {
  public:
    PlaneMesh(glm::vec2 size, glm::vec2 uvTile = glm::vec2(1.0f, 1.0f));
    ~PlaneMesh();
  };
}

#endif // PLANEMESH_H
