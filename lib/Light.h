#ifndef LIGHT_H
#define LIGHT_H

#include "SceneObject.h"
#include "Types.h"

namespace SimpleGL {
  class LightPrivate;

  class Light : public SceneObject {
  public:
    Light();
    virtual ~Light();

    const Vector3f &color() const;
    void setColor(const Vector3f &color);
    void setColor(const float r, const float g, const float b);

    const float diffuseIntensity() const;
    void setDiffuseIntensity(const float intensity);

    const float specularIntensity() const;
    void setSpecularIntensity(const float intensity);

    virtual void render(CameraPtr camera) = 0;

  private:
    LightPrivate *d;
  };
}
#endif // LIGHT_H
