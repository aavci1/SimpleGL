#ifndef LIGHT_H
#define LIGHT_H

#include "Types.h"

namespace SimpleGL {
  class LightPrivate;
  class Camera;

  class Light {
  public:
    Light(LightType type = LT_UNKNOWN);
    ~Light();

    const LightType type() const;

    void setColor(const glm::vec3 &color);
    void setColor(const float r, const float g, const float b);
    const glm::vec3 &color() const;

    void setDiffuseIntensity(const float intensity);
    const float diffuseIntensity() const;

    void setSpecularIntensity(const float intensity);
    const float specularIntensity() const;

    void setEnabled(const bool enabled);
    const bool isEnabled() const;

    virtual const bool isVisibleFrom(Camera *camera) const = 0;

    virtual void render(Camera *camera) = 0;

  private:
    LightPrivate *d;
  };
}

#endif // LIGHT_H
