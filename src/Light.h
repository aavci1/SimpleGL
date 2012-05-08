#ifndef LIGHT_H
#define LIGHT_H

#include "Renderable.h"
#include "SceneObject.h"
#include "Types.h"

namespace SimpleGL {
  class LightPrivate;

  class Light : public Renderable, public SceneObject {
  public:
    Light();
    virtual ~Light();

    virtual const LightType type() const = 0;

    const Vector3f &color() const;
    void setColor(const Vector3f &color);
    void setColor(const float r, const float g, const float b);

    const float diffuseIntensity() const;
    void setDiffuseIntensity(const float intensity);

    const float specularIntensity() const;
    void setSpecularIntensity(const float intensity);

  private:
    LightPrivate *d;
  };
}
#endif // LIGHT_H
