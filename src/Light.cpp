#include "Light.h"

namespace SimpleGL {
  class LightPrivate {
  public:
    LightPrivate() : color(1.0f, 1.0f, 1.0f), diffuseIntensity(1.0f), specularIntensity(1.0f) {
    }

    ~LightPrivate() {
    }

    Vector3f color;
    float diffuseIntensity;
    float specularIntensity;
  };

  Light::Light() : d(new LightPrivate()) {
  }

  Light::~Light() {
    delete d;
  }

  const Vector3f &Light::color() const {
    return d->color;
  }

  void Light::setColor(const Vector3f &color) {
    d->color = color;
  }

  void Light::setColor(const float r, const float g, const float b) {
    setColor(Vector3f(r, g, b));
  }

  const float Light::diffuseIntensity() const {
    return d->diffuseIntensity;
  }

  void Light::setDiffuseIntensity(const float intensity) {
    d->diffuseIntensity = intensity;
  }

  const float Light::specularIntensity() const {
    return d->specularIntensity;
  }

  void Light::setSpecularIntensity(const float intensity) {
    d->specularIntensity = intensity;
  }
}
