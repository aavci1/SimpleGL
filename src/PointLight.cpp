#include "PointLight.h"

#include <glm/ext.hpp>

namespace SimpleGL {
  class PointLightPrivate {
  public:
    PointLightPrivate() : position(0.0f, 0.0f, 0.0f), radius(256.0f), recalcTransformationMatrix(false), transformationMatrix(glm::mat4()) {
    }
    ~PointLightPrivate() {
    }

    glm::vec3 position;
    float radius;
    bool recalcTransformationMatrix;
    glm::mat4 transformationMatrix;
  };

  PointLight::PointLight() : Light(LT_POINT), d(new PointLightPrivate()) {
  }

  PointLight::~PointLight() {
    delete d;
  }

  void PointLight::setPosition(const glm::vec3 &position) {
    d->position = position;
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void PointLight::setPosition(const float x, const float y, const float z) {
    d->position = glm::vec3(x, y, z);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::vec3 &PointLight::position() const {
    return d->position;
  }

  void PointLight::setRadius(const float radius) {
    d->radius = radius;
  }

  const float PointLight::radius() const {
    return d->radius;
  }

  const glm::mat4 &PointLight::transformationMatrix() const {
    if (d->recalcTransformationMatrix) {
      glm::mat4 identity;
      // calculcate transformation matrix
      d->transformationMatrix = glm::translate(identity, d->position);
      // reset flag
      d->recalcTransformationMatrix = false;
    }
    return d->transformationMatrix;
  }
}
