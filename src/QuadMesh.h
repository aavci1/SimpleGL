#ifndef QUADMESH_H
#define QUADMESH_H

#include "Mesh.h"

#include <glm/glm.hpp>

namespace SimpleGL {
  class QuadMesh : public Mesh {
  public:
    QuadMesh(glm::vec2 size = glm::vec2(1.0f, 1.0f));
  };
}

#endif // QUADMESH_H
