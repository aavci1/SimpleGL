#include "DirectionalLight.h"

#include <glm/ext.hpp>

namespace SimpleGL {
  class DirectionalLightPrivate {
  public:
    DirectionalLightPrivate() : direction(0.0f, 0.0f, 1.0f) {
    }
    ~DirectionalLightPrivate() {
    }

    glm::vec3 direction;
  };

  DirectionalLight::DirectionalLight() : Light(LT_DIRECTIONAL), d(new DirectionalLightPrivate()) {
  }

  DirectionalLight::~DirectionalLight() {
    delete d;
  }

  void DirectionalLight::setDirection(const glm::vec3 &direction) {
    d->direction = glm::normalize(direction);
  }

  void DirectionalLight::setDirection(const float x, const float y, const float z) {
    d->direction = glm::normalize(glm::vec3(x, y, z));
  }

  const glm::vec3 &DirectionalLight::direction() const {
    return d->direction;
  }
}