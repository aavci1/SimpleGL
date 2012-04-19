#ifndef SPHERE_H
#define SPHERE_H

#include "Types.h"

namespace SimpleGL {
  class SpherePrivate;

  class Sphere {
  public:
    Sphere(const float radius = 1.0f, const glm::vec3 &center = glm::vec3(0.0f, 0.0f, 0.0f));
    ~Sphere();

    void setRadius(const float radius);
    const float radius() const;

    void setCenter(const glm::vec3 &center);
    const glm::vec3 &center() const;

    const bool intersects(const Sphere *sphere) const;

  private:
    SpherePrivate *d;
  };
}

#endif // SPHERE_H
