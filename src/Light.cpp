#include "Light.h"

namespace SimpleGL {
  class LightPrivate {
  public:
    LightPrivate() : type(LT_POINT), position(0, 0, 0), direction(0, 0, 1), ambientColor(0, 0, 0), diffuseColor(1, 1, 1), specularColor(1, 1, 1) {
    }

    ~LightPrivate() {
    }

    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
  };

  Light::Light() : d(new LightPrivate()) {
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

  const glm::vec3 Light::position() const {
    return d->position;
  }

  void Light::setPosition(const glm::vec3 &position) {
    d->position = position;
  }

  const glm::vec3 Light::direction() const {
    return d->direction;
  }

  void Light::setDirection(const glm::vec3 &direction) {
    d->direction = direction;
  }

  const glm::vec3 Light::ambientColor() const {
    return d->ambientColor;
  }

  void Light::setAmbientColor(const glm::vec3 &color) {
    d->ambientColor = color;
  }

  const glm::vec3 Light::diffuseColor() const {
    return d->diffuseColor;
  }

  void Light::setDiffuseColor(const glm::vec3 &color) {
    d->diffuseColor = color;
  }

  const glm::vec3 Light::specularColor() const {
    return d->specularColor;
  }

  void Light::setSpecularColor(const glm::vec3 &color) {
    d->specularColor = color;
  }
}
