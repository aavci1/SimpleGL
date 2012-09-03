#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

namespace SimpleGL {
  class SpotLightPrivate;

  class SpotLight : public Light {
  public:
    SpotLight();
    ~SpotLight();

    string type() const;

    const Vector3f &direction() const;
    void setDirection(const Vector3f &direction);
    void setDirection(const float x, const float y, const float z);

    const float innerAngle() const;
    void setInnerAngle(const float angle);

    const float outerAngle() const;
    void setOuterAngle(const float angle);

    const float attenuationRange() const;
    const float attenuationConstant() const;
    const float attenuationLinear() const;
    const float attenuationQuadratic() const;
    void setAttenuation(const float range, const float constant = 0.0f, const float linear = 0.0f, const float quadratic = 1.0f);

    void render(shared_ptr<Camera> camera);

  private:
    SpotLightPrivate *d;
  };
}

#endif // SPOTLIGHT_H
