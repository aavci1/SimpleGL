#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

namespace SimpleGL {
  class PointLightPrivate;
  class Camera;

  class PointLight : public Light {
  public:
    PointLight();
    ~PointLight();

    void setPosition(const glm::vec3 &position);
    void setPosition(const float x, const float y, const float z);
    const glm::vec3 &position() const;

    void setAttenuation(float range, float constant = 0.0f, float linear = 0.0f , float quadratic = 1.0f);
    float attenuationRange() const;
    float attenuationConstant() const;
    float attenuationLinear() const;
    float attenuationQuadratic() const;

    const glm::mat4 &transformationMatrix() const;

    const bool isVisibleFrom(Camera *camera) const;

    void render(Camera *camera);

  private:
    PointLightPrivate *d;
  };
}

#endif // POINTLIGHT_H
