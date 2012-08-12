#include "DirectionalLight.h"

#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Program.h"
#include "Root.h"
#include "SceneNode.h"

namespace SimpleGL {
  class DirectionalLightPrivate {
  public:
    DirectionalLightPrivate() {
      quad = Root::instance()->createQuad("");
    }

    ~DirectionalLightPrivate() {
    }

    Mesh *quad { nullptr };
    Vector3f direction { 0.0f, 0.0f, -1.0f };
  };

  DirectionalLight::DirectionalLight() : d(new DirectionalLightPrivate())  {
  }

  DirectionalLight::~DirectionalLight() {
    delete d;
  }

  const LightType DirectionalLight::type() const {
    return LT_DIRECTIONAL;
  }

  const Vector3f &DirectionalLight::direction() const {
    return d->direction;
  }

  void DirectionalLight::setDirection(const Vector3f &direction) {
    d->direction = glm::normalize(direction);
  }

  void DirectionalLight::setDirection(const float x, const float y, const float z) {
    setDirection(Vector3f(x, y, z));
  }

  void DirectionalLight::render(Camera *camera) {
    // get program of the material
    Program *program = Root::instance()->retrieveProgram("DirectionalLight");
    if (!program)
      return;
    // set light properties
    program->setUniform("lightDir", d->direction);
    program->setUniform("lightColor", color());
    program->setUniform("lightDiffuseIntensity", diffuseIntensity());
    program->setUniform("lightSpecularIntensity", specularIntensity());
    // render a full screen quad
    d->quad->render(camera);
  }
}
