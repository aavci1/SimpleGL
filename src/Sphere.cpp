#include "Sphere.h"

namespace SimpleGL {
  class SpherePrivate {
  public:
    SpherePrivate(const float radius, const glm::vec3 &center) : radius(radius), center(center) {
    }
    ~SpherePrivate() {
    }

    float radius;
    glm::vec3 center;
  };

  Sphere::Sphere(const float radius, const glm::vec3 &center) : d(new SpherePrivate(radius, center)) {
  }

  Sphere::~Sphere() {
    delete d;
  }

  void Sphere::setRadius(const float radius) {
    d->radius = radius;
  }

  const float Sphere::radius() const {
    return d->radius;
  }

  void Sphere::setCenter(const glm::vec3 &center) {
    d->center = center;
  }

  const glm::vec3 &Sphere::center() const {
    return d->center;
  }

  const bool Sphere::intersects(const Sphere *sphere) const {
    // vector from the center of this sphere to the others
    glm::vec3 distance = d->center - sphere->center();
    // sum of radiuses
    float radiusSum = d->radius + sphere->radius();
    // if distance squared is smaller than radiusSum squared, return true
    if (glm::length2(distance) < radiusSum * radiusSum)
      return true;
    // return false
    return false;
  }
}
