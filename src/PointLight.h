#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

namespace SimpleGL {
  class PointLightPrivate;

  class PointLight : public Light {
  public:
    PointLight();
    ~PointLight();

    void setPosition(const glm::vec3 &position);
    void setPosition(const float x, const float y, const float z);
    const glm::vec3 &position() const;

    void setRadius(const float radius);
    const float radius() const;

    const glm::mat4 &transformationMatrix() const;

  private:
    PointLightPrivate *d;
  };
}

#endif // POINTLIGHT_H
