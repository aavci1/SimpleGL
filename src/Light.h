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
    Light();
    ~Light();

    const LightType type() const;
    void setType(LightType type);

    const glm::vec3 position() const;
    void setPosition(const glm::vec3 &position);

    const glm::vec3 direction() const;
    void setDirection(const glm::vec3 &direction);

    const glm::vec3 ambientColor() const;
    void setAmbientColor(const glm::vec3 &color);

    const glm::vec3 diffuseColor() const;
    void setDiffuseColor(const glm::vec3 &color);

    const glm::vec3 specularColor() const;
    void setSpecularColor(const glm::vec3 &color);

  private:
    LightPrivate *d;
  };
}

#endif // LIGHT_H
