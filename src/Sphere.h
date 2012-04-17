#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"

#include <glm/glm.hpp>

namespace SimpleGL {
  class Sphere : public Mesh {
  public:
    Sphere(float r = 1.0f, int slices = 16, int stacks = 16);
    ~Sphere();

    void render();
  };
}

#endif // SPHERE_H
