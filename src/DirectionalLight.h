#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

namespace SimpleGL {
  class DirectionalLightPrivate;

  class DirectionalLight : public Light {
  public:
    DirectionalLight();
    ~DirectionalLight();

    void setDirection(const glm::vec3 &direction);
    void setDirection(const float x, const float y, const float z);
    const glm::vec3 &direction() const;

  private:
    DirectionalLightPrivate *d;
  };
}

#endif // DIRECTIONALLIGHT_H
