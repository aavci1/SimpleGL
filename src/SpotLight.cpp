#include "SpotLight.h"

#include "Camera.h"
#include "Material.h"
#include "MaterialManager.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "Program.h"
#include "SubMesh.h"

#include <GL/glew.h>

#include <glm/ext.hpp>

namespace SimpleGL {
  class SpotLightPrivate {
  public:
    SpotLightPrivate() : position(0.0f, 0.0f, 0.0f), orientation(1, 0, 0, 0), direction(0, 0, -1), innerAngle(30), outerAngle(30), attenuationRange(256.0f), attenuationConstant(0.0f), attenuationLinear(0.0f), attenuationQuadratic(1.0f), recalcTransformationMatrix(false), transformationMatrix(glm::mat4()) {
      cone = MeshManager::instance()->createCone(tanf(innerAngle + outerAngle) * attenuationRange, attenuationRange);
    }

    ~SpotLightPrivate() {
      delete cone;
    }

    Mesh *cone;
    glm::vec3 position;
    glm::quat orientation;
    glm::vec3 direction;
    float innerAngle;
    float outerAngle;
    float attenuationRange;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
    bool recalcTransformationMatrix;
    glm::mat4 transformationMatrix;
  };

  SpotLight::SpotLight() : Light(LT_SPOT), d(new SpotLightPrivate()) {
  }

  SpotLight::~SpotLight() {
    delete d;
  }

  void SpotLight::setPosition(const glm::vec3 &position) {
    d->position = position;
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  void SpotLight::setPosition(const float x, const float y, const float z) {
    d->position = glm::vec3(x, y, z);
    // transformation matrix needs to be recalculated
    d->recalcTransformationMatrix = true;
  }

  const glm::vec3 &SpotLight::position() const {
    return d->position;
  }

  void SpotLight::setOrientation(const glm::quat &orientation) {
    d->orientation = glm::normalize(orientation);
    // recalc view matrix
    d->recalcTransformationMatrix = true;
  }

  void SpotLight::setOrientation(float w, float x, float y, float z) {
    d->orientation = glm::normalize(glm::quat(w, x, y, z));
    // recalc view matrix
    d->recalcTransformationMatrix = true;
  }

  const glm::quat &SpotLight::orientation() const {
    return d->orientation;
  }

  void SpotLight::rotate(float angle, const glm::vec3 &axis, TransformSpace transformSpace) {
    if (transformSpace == TS_WORLD)
      setOrientation(glm::angleAxis(angle, axis) * d->orientation);
    else if (transformSpace == TS_LOCAL)
      setOrientation(d->orientation * glm::angleAxis(angle, axis));
  }

  void SpotLight::pitch(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f), transformSpace);
  }

  void SpotLight::yaw(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f), transformSpace);
  }

  void SpotLight::roll(float angle, TransformSpace transformSpace) {
    rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f), transformSpace);
  }


  const float SpotLight::innerAngle() const {
    return d->innerAngle;
  }

  void SpotLight::setInnerAngle(const float angle) {
    d->innerAngle = angle;
  }

  const float SpotLight::outerAngle() const {
    return d->outerAngle;
  }

  void SpotLight::setOuterAngle(const float angle) {
    d->outerAngle = angle;
    // re-create cone
    delete d->cone;
    d->cone = MeshManager::instance()->createCone(tanf((d->innerAngle + d->outerAngle) * M_PI / 180) * d->attenuationRange, d->attenuationRange);
  }

  void SpotLight::setAttenuation(float range, float constant, float linear, float quadratic) {
    d->attenuationRange = range;
    d->attenuationConstant = constant;
    d->attenuationLinear = linear;
    d->attenuationQuadratic = quadratic;
    // re-create cone
    delete d->cone;
    d->cone = MeshManager::instance()->createCone(tanf((d->innerAngle + d->outerAngle) * M_PI / 180) * d->attenuationRange, d->attenuationRange);
  }

  float SpotLight::attenuationRange() const {
    return d->attenuationRange;
  }

  float SpotLight::attenuationConstant() const {
    return d->attenuationConstant;
  }

  float SpotLight::attenuationLinear() const {
    return d->attenuationLinear;
  }

  float SpotLight::attenuationQuadratic() const {
    return d->attenuationQuadratic;
  }

  const glm::mat4 &SpotLight::transformationMatrix() const {
    if (d->recalcTransformationMatrix) {
      glm::mat4 identity;
      // calculcate transformation matrix
      d->transformationMatrix = glm::translate(identity, d->position) * glm::mat4_cast(d->orientation);
      // reset flag
      d->recalcTransformationMatrix = false;
    }
    return d->transformationMatrix;
  }

  const bool SpotLight::isVisibleFrom(Camera *camera) const {
    // TODO: implement frustum culling
    return true;
  }

  void SpotLight::render(Camera *camera) {
    Material *material = MaterialManager::instance()->getMaterialByLightType(type());
    // check material
    if (!material || !material->program())
      return;
    // adjust face culling
    glm::vec3 direction = d->orientation * d->direction;
    float cos = cosf((d->innerAngle + d->outerAngle) * M_PI / 180);
    glm::vec3 cameraDir = glm::normalize(camera->position() - d->position);
    if (glm::dot(cameraDir, direction) >= cos)
      glCullFace(GL_FRONT);
    else
      glCullFace(GL_BACK);
    // set program parameters
    material->program()->setUniform("lightColor", color());
    material->program()->setUniform("lightDiffuseIntensity", diffuseIntensity());
    material->program()->setUniform("lightSpecularIntensity", specularIntensity());
    material->program()->setUniform("lightPos", d->position);
    material->program()->setUniform("lightDirection", direction);
    material->program()->setUniform("lightInnerAngle", float(d->innerAngle * M_PI / 180.0f));
    material->program()->setUniform("lightOuterAngle", float(d->outerAngle * M_PI / 180.0f));
    material->program()->setUniform("lightAttenuationRange", d->attenuationRange);
    material->program()->setUniform("lightAttenuationConstant", d->attenuationConstant);
    material->program()->setUniform("lightAttenuationLinear", d->attenuationLinear);
    material->program()->setUniform("lightAttenuationQuadratic", d->attenuationQuadratic);
    material->program()->setUniform("modelViewProjMatrix", camera->projectionMatrix() * camera->viewMatrix() * transformationMatrix());
    // render a cone
    d->cone->subMeshes().at(0)->render();
    // reset flags
    glCullFace(GL_BACK);
  }
}
