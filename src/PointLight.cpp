#include "PointLight.h"

#include <glm/ext.hpp>

namespace SimpleGL {
  class PointLightPrivate {
  public:
    PointLightPrivate() : position(0.0f, 0.0f, 0.0f), attenuationRange(256.0f), attenuationConstant(0.0f), attenuationLinear(0.0f), attenuationQuadratic(1.0f), recalcTransformationMatrix(false), transformationMatrix(glm::mat4()) {
    }
    ~PointLightPrivate() {
    }

    glm::vec3 position;
    float attenuationRange;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
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

  void PointLight::setAttenuation(float range, float constant, float linear, float quadratic) {
    d->attenuationRange = range;
    d->attenuationConstant = constant;
    d->attenuationLinear = linear;
    d->attenuationQuadratic = quadratic;
  }

  float PointLight::attenuationRange() const {
    return d->attenuationRange;
  }

  float PointLight::attenuationConstant() const {
    return d->attenuationConstant;
  }

  float PointLight::attenuationLinear() const {
    return d->attenuationLinear;
  }

  float PointLight::attenuationQuadratic() const {
    return d->attenuationQuadratic;
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
