#include "DirectionalLight.h"

#include "Camera.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "Program.h"
#include "SubMesh.h"

#include <glm/ext.hpp>

namespace SimpleGL {
  class DirectionalLightPrivate {
  public:
    DirectionalLightPrivate() : direction(0.0f, 0.0f, 1.0f) {
      quad = MeshManager::instance()->createQuad();
    }

    ~DirectionalLightPrivate() {
      delete quad;
    }

    Mesh *quad;
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

  const bool DirectionalLight::isVisibleFrom(Camera *camera) const {
    return true;
  }

  void DirectionalLight::render(Camera *camera) {
    Program *program = MaterialManager::instance()->getMaterialByLightType(type())->program();
    // return if program is not set
    if (!program)
      return;
    // set program parameters
    program->setUniform("lightDir", d->direction);
    program->setUniform("lightColor", color());
    program->setUniform("lightDiffuseIntensity", diffuseIntensity());
    program->setUniform("lightSpecularIntensity", specularIntensity());
    // render a full screen quad
    d->quad->subMeshes().at(0)->render();
  }
}
