#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Light.h"

namespace SimpleGL {
  class DirectionalLightPrivate;

  class DirectionalLight : public Light {
  public:
    DirectionalLight();
    ~DirectionalLight();

    string type() const;

    const Vector3f &direction() const;
    void setDirection(const Vector3f &direction);
    void setDirection(const float x, const float y, const float z);

    void render(shared_ptr<Camera> camera);

  private:
    DirectionalLightPrivate *d;
  };
}

#endif // DIRECTIONALLIGHT_H
