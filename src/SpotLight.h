#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"
#include "Types.h"

namespace SimpleGL {
  class SpotLightPrivate;

  class SpotLight : public Light {
  public:
    SpotLight();
    ~SpotLight();

    void setPosition(const glm::vec3 &position);
    void setPosition(const float x, const float y, const float z);
    const glm::vec3 &position() const;

    void setOrientation(const glm::quat &orientation);
    void setOrientation(float w, float x, float y, float z);
    const glm::quat &orientation() const;

    void rotate(float angle, const glm::vec3 &axis, TransformSpace transformSpace = TS_LOCAL);

    void pitch(float angle, TransformSpace transformSpace = TS_LOCAL);
    void yaw(float angle, TransformSpace transformSpace = TS_WORLD);
    void roll(float angle, TransformSpace transformSpace = TS_LOCAL);

    const float innerAngle() const;
    void setInnerAngle(const float angle);

    const float outerAngle() const;
    void setOuterAngle(const float angle);

    void setAttenuation(float range, float constant = 0.0f, float linear = 0.0f , float quadratic = 1.0f);
    float attenuationRange() const;
    float attenuationConstant() const;
    float attenuationLinear() const;
    float attenuationQuadratic() const;

    const glm::mat4 &transformationMatrix() const;


    const bool isVisibleFrom(Camera *camera) const;

    void render(Camera *camera);

  private:
    SpotLightPrivate *d;
  };
}

#endif // SPOTLIGHT_H
