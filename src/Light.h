#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace SimpleGL {
  class LightPrivate;

  enum LightType {
    LT_DIRECTIONAL,
    LT_POINT,
    LT_SPOT
  };

  class Light {
  public:
    Light(LightType type = LT_POINT);
    ~Light();

    const LightType type() const;
    void setType(LightType type);

    void setPosition(const glm::vec3 &position);
    void setPosition(const float x, const float y, const float z);
    const glm::vec3 &position() const;

    void setDirection(const glm::vec3 &direction);
    void setDirection(const float x, const float y, const float z);
    const glm::vec3 &direction() const;

    void setColor(const glm::vec3 &color);
    void setColor(const float r, const float g, const float b);
    const glm::vec3 &color() const;

    void setDiffuseIntensity(const float intensity);
    const float diffuseIntensity() const;

    void setSpecularIntensity(const float intensity);
    const float specularIntensity() const;

    void setRadius(const float radius);
    const float radius() const;

    const glm::mat4 &transformationMatrix() const;

  private:
    LightPrivate *d;
  };
}

#endif // LIGHT_H
