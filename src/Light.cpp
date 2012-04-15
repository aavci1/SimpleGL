#include "Light.h"

#include <glm/ext.hpp>

namespace SimpleGL {
  class LightPrivate {
  public:
    LightPrivate(LightType type) : type(type), position(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 1.0f), color(1.0f, 1.0f, 1.0f), diffuseIntensity(1.0f), specularIntensity(1.0f), radius(256.0f), recalcTransformationMatrix(false) {
    }

    ~LightPrivate() {
    }

    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float diffuseIntensity;
    float specularIntensity;
    float radius;
    bool recalcTransformationMatrix;
    glm::mat4 transformationMatrix;
  };

  Light::Light(LightType type) : d(new LightPrivate(type)) {
  }

  Light::~Light() {
    delete d;
  }

  const LightType Light::type() const {
    return d->type;
  }

  void Light::setType(LightType type) {
    d->type = type;
  }

  void Light::setPosition(const glm::vec3 &position) {
    d->position = position;
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void Light::setPosition(const float x, const float y, const float z) {
    d->position = glm::vec3(x, y, z);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::vec3 &Light::position() const {
    return d->position;
  }

  void Light::setDirection(const glm::vec3 &direction) {
    d->direction = glm::normalize(direction);
  }

  void Light::setDirection(const float x, const float y, const float z) {
    d->direction = glm::normalize(glm::vec3(x, y, z));
  }

  const glm::vec3 &Light::direction() const {
    return d->direction;
  }

  void Light::setColor(const glm::vec3 &color) {
    d->color = color;
  }

  void Light::setColor(const float r, const float g, const float b) {
    d->color = glm::vec3(r, g, b);
  }

  const glm::vec3 &Light::color() const {
    return d->color;
  }

  void Light::setDiffuseIntensity(const float intensity) {
    d->diffuseIntensity = intensity;
  }

  const float Light::diffuseIntensity() const {
    return d->diffuseIntensity;
  }

  void Light::setSpecularIntensity(const float intensity) {
    d->specularIntensity = intensity;
  }

  const float Light::specularIntensity() const {
    return d->specularIntensity;
  }

  void Light::setRadius(const float radius) {
    d->radius = radius;
  }

  const float Light::radius() const {
    return d->radius;
  }

  const glm::mat4 &Light::transformationMatrix() const {
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
