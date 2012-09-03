#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

namespace SimpleGL {
  class PointLightPrivate;

  class PointLight : public Light {
  public:
    PointLight();
    ~PointLight();

    string type() const;

    const float attenuationRange() const;
    const float attenuationConstant() const;
    const float attenuationLinear() const;
    const float attenuationQuadratic() const;
    void setAttenuation(const float range, const float constant = 0.0f, const float linear = 0.0f, const float quadratic = 1.0f);

    void render(CameraPtr camera);

  private:
    PointLightPrivate *d;
  };
}


#endif // POINTLIGHT_H
