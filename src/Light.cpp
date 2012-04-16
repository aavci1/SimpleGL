#include "Light.h"

namespace SimpleGL {
  class LightPrivate {
  public:
    LightPrivate(LightType type) : type(type), color(1.0f, 1.0f, 1.0f), diffuseIntensity(1.0f), specularIntensity(1.0f), enabled(true) {
    }

    ~LightPrivate() {
    }

    LightType type;
    glm::vec3 color;
    float diffuseIntensity;
    float specularIntensity;
    bool enabled;
  };

  Light::Light(LightType type) : d(new LightPrivate(type)) {
  }

  Light::~Light() {
    delete d;
  }

  const LightType Light::type() const {
    return d->type;
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

  void Light::setEnabled(const bool enabled) {
    d->enabled = enabled;
  }

  const bool Light::isEnabled() const {
    return d->enabled;
  }
}
