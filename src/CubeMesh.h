#ifndef CUBEMESH_H
#define CUBEMESH_H

#include "Mesh.h"

#include <glm/glm.hpp>

namespace SimpleGL {
  class CubeMesh : public Mesh {
  public:
    CubeMesh(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
    ~CubeMesh();
  };
}

#endif // CUBEMESH_H
