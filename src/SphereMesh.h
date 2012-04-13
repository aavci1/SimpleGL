#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "Mesh.h"

#include <glm/glm.hpp>

namespace SimpleGL {
  class SphereMesh : public Mesh {
  public:
    SphereMesh(float r = 1.0f, int slices = 16, int stacks = 16);
    ~SphereMesh();
  };
}

#endif // SPHEREMESH_H
