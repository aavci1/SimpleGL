#include "Light.h"

namespace SimpleGL {
  class LightPrivate {
  public:
    LightPrivate() {
    }

    ~LightPrivate() {
    }

    Vector3f color { 1.0f, 1.0f, 1.0f };
    float diffuseIntensity { 1.0f };
    float specularIntensity { 1.0f };
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
