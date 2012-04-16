#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace SimpleGL {
  class LightPrivate;

  enum LightType {
    LT_UNKNOWN,
    LT_DIRECTIONAL,
    LT_POINT,
    LT_SPOT
  };

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

    virtual const glm::mat4 &transformationMatrix() const;

  private:
    LightPrivate *d;
  };
}

#endif // LIGHT_H
